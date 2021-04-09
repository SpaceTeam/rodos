#include <new>
#include "rodos.h"
#include "hal/hal_spi.h"
#include "hw_hal_gpio.h"


#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

namespace RODOS {
/** TODO:
 * - timeout for while loops
 * - read/write not "busy-waiting" -> interrupts
 * - reset()
 * - status()
 * - config()
 */

#define SPI_IDX_MIN SPI_IDX1
#define SPI_IDX_MAX SPI_IDX2


class HW_HAL_SPI {
public:
	SPI_IDX idx;
	bool initialized;
	USART_TypeDef *SPIx;
	uint32_t baudrate;
	USART_InitSync_TypeDef config;

	uint32_t GPIO_Pin_MISO;
	uint32_t GPIO_Pin_MOSI;
	uint32_t GPIO_Pin_SCK;
	uint32_t GPIO_Pin_NSS;
	GPIO_Port_TypeDef GPIO_Port_MISO;
	GPIO_Port_TypeDef GPIO_Port_MOSI;
	GPIO_Port_TypeDef GPIO_Port_SCK;
	GPIO_Port_TypeDef GPIO_Port_NSS;


public:
    HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin);    
	HW_HAL_SPI(SPI_IDX idx);                                                                        

	HW_HAL_SPI() {
	// this constructor is called to init SPIcontextArray-objects
	// !!! don't set member variables -> this might overwrite values already set by constructor call of placement new
	}

	~HW_HAL_SPI(){}

	void waitOnTransferReady();

	uint32_t setBaudrate(uint32_t baudrate);

    void initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin = GPIO_INVALID);

};



HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) { 
  RODOS_ASSERT(idx >= SPI_IDX_MIN); // SPI index out of range
  RODOS_ASSERT(idx <= SPI_IDX_MAX); // SPI index out of range
  this->initialized = false;
  initMembers(idx, sckPin, misoPin, mosiPin, nssPin);
}


HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx) {

	switch(idx){
	case SPI_IDX1:
    //SCK = PC2, MISO = PC1, MOSI = PC0, CS = PC3    - TO COMUNICATE WITH LSM9DS1  -   accelerometer and gyroscope 
	//SCK = PC2, MISO = PC1, MOSI = PC0, CS = PC4   - TO COMUNICATE WITH LSM9DS1  -     magnetometer                              
    initMembers(idx, GPIO_034, GPIO_033, GPIO_032 );    ///////----------------------------------------------------------------------- /////////////////HAL_GPIO.H / SCHEMATICK 
		break;
    case SPI_IDX2:
	//SCK = PF5(GPIO_085), MISO = PF4(GPIO_084), MOSI = PF3(GPIO_083), CS = PC10(GPIO_042)  -TO COMUNICATE WITH RAM
	//SCK = PF5(GPIO_085), MISO = PF4(GPIO_084), MOSI = PF3(GPIO_083), CS = PC11(GPIO_043)  -TO COMUNICATE WITH FLASH1
	//SCK = PF5(GPIO_085), MISO = PF4(GPIO_084), MOSI = PF3(GPIO_083), CS = PB9 (GPIO_025)  -TO COMUNICATE WITH FLASH2
	//SCK = PF5(GPIO_085), MISO = PF4(GPIO_084), MOSI = PF3(GPIO_083), CS = PB10(GPIO_026)  -TO COMUNICATE WITH FLASH3
	initMembers(idx,GPIO_085 ,GPIO_084 ,GPIO_083 );
		break;
	default:
            RODOS_ERROR("SPI index out of range");
	}
}



void HW_HAL_SPI::initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {

	this->idx = idx;

	switch(idx){
	case SPI_IDX1:
		CMU_ClockEnable(cmuClock_GPIO, true);
  		CMU_ClockEnable(cmuClock_USART3, true);
		break;
	case SPI_IDX2:
		CMU_ClockEnable(cmuClock_GPIO, true);
  		CMU_ClockEnable(cmuClock_USART2, true);
		break;
	default:
		RODOS_ERROR("SPI index out of range");
	}



    GPIO_Pin_SCK   = HW_HAL_GPIO::getEFR32Pin(sckPin);
	GPIO_Port_SCK  = HW_HAL_GPIO::getEFR32Port(sckPin);

	GPIO_Pin_MISO  = HW_HAL_GPIO::getEFR32Pin(misoPin);
	GPIO_Port_MISO = HW_HAL_GPIO::getEFR32Port(misoPin);

	GPIO_Pin_NSS  = HW_HAL_GPIO::getEFR32Pin(nssPin);
	GPIO_Port_NSS = HW_HAL_GPIO::getEFR32Port(nssPin);

	GPIO_Pin_MOSI  = HW_HAL_GPIO::getEFR32Pin(mosiPin);
	GPIO_Port_MOSI = HW_HAL_GPIO::getEFR32Port(mosiPin);
	

	//SPI_IDX1
	//GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0); // US3_CLK is push pull
  	//GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1); // US3_TX (MOSI) is push pull
  	//GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 1);    // US3_RX (MISO) is input

	//SPI_IDX2
	//GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0); // US2_CLK is push pull
  	//GPIO_PinModeSet(gpioPortF, 3, gpioModePushPull, 1); // US2_TX (MOSI) is push pull
  	//GPIO_PinModeSet(gpioPortF, 4, gpioModeInput, 1);    // US2_RX (MISO) is input


	switch(idx){
	case SPI_IDX1:
		SPIx = USART3;
		config.enable  = usartDisable;
		break;
	case SPI_IDX2:
		SPIx = USART2;
		config.enable  = usartDisable;
		break;
	default:
		RODOS_ERROR("SPI index out of range");
	}

	GPIO_PinModeSet(GPIO_Port_SCK, GPIO_Pin_SCK, gpioModePushPull, 0);
	GPIO_PinModeSet(GPIO_Port_MOSI, GPIO_Pin_MOSI, gpioModePushPull, 1); 
	GPIO_PinModeSet(GPIO_Port_MISO, GPIO_Pin_MISO, gpioModeInput, 1);	
	
}



uint32_t HW_HAL_SPI::setBaudrate(uint32_t baudrate){

	CMU_ClockEnable(cmuClock_GPIO, true);
  	CMU_ClockEnable(cmuClock_USART3, true);

	config.enable   = usartDisable;

	config.baudrate	= baudrate; 

	USART_InitSync(SPIx, &config);	

    return 0;
}

HW_HAL_SPI SPIcontextArray[SPI_IDX_MAX];

//HAL_SPI::~HAL_SPI(){}
    

HAL_SPI::HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {
	if (idx < SPI_IDX_MIN || idx > SPI_IDX_MAX) {
		RODOS_ERROR("SPI index out of range");
	} else {
		context = new (&SPIcontextArray[idx]) HW_HAL_SPI(idx,sckPin,misoPin,mosiPin, nssPin); 
	}
}

HAL_SPI::HAL_SPI(SPI_IDX idx) {
	if (idx < SPI_IDX_MIN || idx > SPI_IDX_MAX) {
		RODOS_ERROR("SPI index out of range");
	} else {
		context = new (&SPIcontextArray[idx]) HW_HAL_SPI(idx);
	}
}

/** init SPI interface
 * @param baudrate
 * @return baudrate that is really configured, considering APB clock and possible baudrate prescaler (2,4,8,16,32,64,128,256)
 */
int32_t HAL_SPI::init(uint32_t baudrate, bool slave, bool tiMode) {       

  	CMU_ClockEnable(cmuClock_GPIO, true);
	/*
	usartClockMode0 	-Clock idle low, sample on rising edge.
	usartClockMode1 	-Clock idle low, sample on falling edge.
	usartClockMode2 	-Clock idle high, sample on falling edge.
	usartClockMode3 	-Clock idle high, sample on rising edge. 
	*/

	switch(context->idx){
	case SPI_IDX1:
  		CMU_ClockEnable(cmuClock_USART3, true);
  
		context->config = USART_INITSYNC_DEFAULT;
		context->config.master       = true;            				
  		context->config.baudrate     = baudrate;         			
  		context->config.autoCsEnable = false;  
		context->config.clockMode = usartClockMode3;
		context->config.msbf      = true;  
		break;

	case SPI_IDX2:
		CMU_ClockEnable(cmuClock_USART2, true);
		context->config = USART_INITSYNC_DEFAULT;
		context->config.master       = true;            				
  		context->config.baudrate     = baudrate;         			
  		context->config.autoCsEnable = false;  
		context->config.clockMode = usartClockMode3;			//mode 3 or 0 can be defined to communicate with flash
		context->config.msbf      = true;  
		break;
	default:
		RODOS_ERROR("SPI index out of range");
	}

  	context->config.enable = usartDisable;    				
  	USART_InitSync(context->SPIx, &(context->config));

	switch(context->idx){
	case SPI_IDX1:
		USART3->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC18) | // US3_CLK       on location 1 = PC2        
                      		(USART_ROUTELOC0_TXLOC_LOC18)  | // US3_TX (MOSI) on location 1 = PC0 
                      		(USART_ROUTELOC0_RXLOC_LOC18);   // US3_RX (MISO) on location 1 = PC1         					
		
  		USART3->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

		break;
	case SPI_IDX2:
		USART2->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC16) | // US2_CLK       on location 1 = PF5      
                      		(USART_ROUTELOC0_TXLOC_LOC16)  | // US2_TX (MOSI) on location 1 = PF3 
                      		(USART_ROUTELOC0_RXLOC_LOC16);   // US2_RX (MISO) on location 1 = PF4         					
		
		USART2->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

		break;
	default:
		RODOS_ERROR("SPI index out of range");
	}

	USART_Enable(context->SPIx, usartEnable);

	context->initialized = true;
	return 0;
}


int32_t HAL_SPI::writeRead(const void* sendBuf, size_t len, void* recBuf, size_t maxLen) {
	if(!context->initialized) return -1;
	
	uint8_t TxBufferIndex;
	uint8_t *send_pointer;
	send_pointer = (uint8_t*)sendBuf;
	uint8_t val;
	uint8_t pp;

	uint8_t RxBufferIndex;
	uint8_t *rec_pointer;
	rec_pointer = (uint8_t*)recBuf;

	//SEND DATA
	for(TxBufferIndex = 0 ; TxBufferIndex < len; TxBufferIndex++){
		val = *(send_pointer + TxBufferIndex);
		pp = USART_SpiTransfer(context->SPIx, val);
	}

	//RECIEVE DATA
    for(RxBufferIndex=0; RxBufferIndex < maxLen; RxBufferIndex++){
		*(rec_pointer + RxBufferIndex) = USART_SpiTransfer(context->SPIx, 0x00); 
	}

	return static_cast<int32_t>(len);
}



int32_t HAL_SPI::write(const void* sendBuf, size_t len) {

	if(!context->initialized) return -1;
	
	uint8_t TxBufferIndex;
	uint8_t *pointer;
	pointer = (uint8_t*)sendBuf;
	uint8_t val;
	uint8_t pp;
	//SEND DATA
	for(TxBufferIndex = 0 ; TxBufferIndex < len; TxBufferIndex++){
		val = *(pointer + TxBufferIndex);
		pp = USART_SpiTransfer(context->SPIx, val);
	}

	return static_cast<int32_t>(len);
}


int32_t HAL_SPI::read(void* recBuf, size_t maxLen) {

	if(!context->initialized) return -1;
 
	uint8_t RxBufferIndex;
	uint8_t *pointer;
	pointer = (uint8_t*)recBuf;

	//RECIEVE DATA
    for(RxBufferIndex=0; RxBufferIndex < maxLen; RxBufferIndex++){
		*(pointer + RxBufferIndex) = USART_SpiTransfer(context->SPIx, 0x00); 
	}

	return static_cast<int32_t>(maxLen);
}

bool HAL_SPI::isWriteFinished(){
	return true;
}

bool HAL_SPI::isReadFinished(){
	return true;
}

}


