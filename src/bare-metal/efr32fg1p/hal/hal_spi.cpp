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
 * - slave-mode, tiMode
 * - DMA
 * - change pins with constructor -> pin routing -> "ROUTELOC0"
 * - use of autoCS-pin
 */

  /* default pin configurations
   * SPI0: SCK = PA2 (LOC0), MISO = PA1 (LOC0), MOSI = PA0 (LOC0)
   * SPI1: SCK = PA5 (LOC3), MISO = PA4 (LOC3), MOSI = PA3 (LOC3)
   * SPI2: SCK = PF5 (LOC16), MISO = PF4 (LOC16), MOSI = PF3 (LOC16)
   * SPI3: SCK = PC2 (LOC18), MISO = PC1 (LOC18), MOSI = PC0 (LOC18)
   */

  // chip selects on "innocube eval board for radiation tests"
  //CS = PC10(GPIO_042)  -TO COMUNICATE WITH RAM
  //CS = PC11(GPIO_043)  -TO COMUNICATE WITH FLASH1
  //CS = PB9 (GPIO_025)  -TO COMUNICATE WITH FLASH2
  //CS = PB10(GPIO_026)  -TO COMUNICATE WITH FLASH3
  //
  //CS = PC3    - TO COMUNICATE WITH LSM9DS1  -   accelerometer and gyroscope
  //CS = PC4   - TO COMUNICATE WITH LSM9DS1  -     magnetometer



#define SPI_IDX_MIN SPI_IDX0
#define SPI_IDX_MAX SPI_IDX3


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

    //int32_t setBaudrate(uint32_t baudrate);
    int32_t enableClocks();

    void initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin = GPIO_INVALID);

};

HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) { 
  RODOS_ASSERT(idx >= SPI_IDX_MIN); // SPI index out of range
  RODOS_ASSERT(idx <= SPI_IDX_MAX); // SPI index out of range
  initMembers(idx, sckPin, misoPin, mosiPin, nssPin);
}


HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx) {

	switch(idx){
	  case SPI_IDX0: // USART0
	    initMembers(idx, GPIO_002, GPIO_001, GPIO_000 ); // SCK = PA2 (LOC0), MISO = PA1 (LOC0), MOSI = PA0 (LOC0)
	    break;

	  case SPI_IDX1: // USART1
	    initMembers(idx, GPIO_005, GPIO_004, GPIO_003 );  // SCK = PA5 (LOC3), MISO = PA4 (LOC3), MOSI = PA3 (LOC3)
	    break;

	  case SPI_IDX2: // USART2
	    initMembers(idx,GPIO_085 ,GPIO_084, GPIO_083 );   // SCK = PF5 (LOC16), MISO = PF4 (LOC16), MOSI = PF3 (LOC16)
	    break;

	  case SPI_IDX3: // USART3
	    initMembers(idx, GPIO_034, GPIO_033, GPIO_032 );  // SCK = PC2 (LOC18), MISO = PC1 (LOC18), MOSI = PC0 (LOC18)
	    break;

	default:
	  RODOS_ERROR("SPI index out of range");
	}
}

void HW_HAL_SPI::initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {

  this->initialized = false;
	this->idx = idx;
  config.enable  = usartDisable;
  SPIx = NULL;

	switch(idx){
	  case SPI_IDX0:
	    SPIx = USART0;
	    break;

	  case SPI_IDX1:
	    SPIx = USART1;
	    break;

	  case SPI_IDX2:
      SPIx = USART2;
  		break;

	  case SPI_IDX3:
      SPIx = USART3;
  		break;

	  default:
	    RODOS_ERROR("SPI index out of range");
	    return;
	}

  GPIO_Pin_SCK   = HW_HAL_GPIO::getEFR32Pin(sckPin);
	GPIO_Port_SCK  = HW_HAL_GPIO::getEFR32Port(sckPin);

	GPIO_Pin_MISO  = HW_HAL_GPIO::getEFR32Pin(misoPin);
	GPIO_Port_MISO = HW_HAL_GPIO::getEFR32Port(misoPin);

	GPIO_Pin_NSS  = HW_HAL_GPIO::getEFR32Pin(nssPin);
	GPIO_Port_NSS = HW_HAL_GPIO::getEFR32Port(nssPin);

	GPIO_Pin_MOSI  = HW_HAL_GPIO::getEFR32Pin(mosiPin);
	GPIO_Port_MOSI = HW_HAL_GPIO::getEFR32Port(mosiPin);
}

//int32_t HW_HAL_SPI::setBaudrate(uint32_t baudrate){
//
//    if (SPIx == NULL){return -1;}
//
//  	config.enable   = usartDisable;
//
//  	config.baudrate	= baudrate;
//
//  	USART_InitSync(SPIx, &config);
//
//    return 0;
//}

int32_t HW_HAL_SPI::enableClocks(){
#if defined(_CMU_HFPERCLKEN0_MASK)
    CMU_ClockEnable(cmuClock_HFPER, true);
#endif

    CMU_ClockEnable(cmuClock_GPIO, true);

    switch(idx){
      case SPI_IDX0:
        CMU_ClockEnable(cmuClock_USART0, true);
        break;

      case SPI_IDX1:
        CMU_ClockEnable(cmuClock_USART1, true);
        break;

      case SPI_IDX2:
        CMU_ClockEnable(cmuClock_USART2, true);
        break;

      case SPI_IDX3:
        CMU_ClockEnable(cmuClock_USART3, true);
        break;

      default:
        RODOS_ERROR("SPI index out of range");
        return -1;
    }

    return 0;
}

HW_HAL_SPI SPIcontextArray[SPI_IDX_MAX+1];








HAL_SPI::~HAL_SPI(){}

HAL_SPI::HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {
	if (idx < SPI_IDX_MIN || idx > SPI_IDX_MAX) {
		RODOS_ERROR("SPI index out of range");
	} else {
		context = new (&SPIcontextArray[idx]) HW_HAL_SPI(idx, sckPin, misoPin, mosiPin, nssPin);
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
 *
 * ToDo: slave-mode, tiMode
 */
int32_t HAL_SPI::init(uint32_t baudrate, bool slave, bool tiMode) {       

    context->enableClocks();

    /*
	  usartClockMode0 	-Clock idle low, sample on rising edge.
	  usartClockMode1 	-Clock idle low, sample on falling edge.
	  usartClockMode2 	-Clock idle high, sample on falling edge.
	  usartClockMode3 	-Clock idle high, sample on rising edge.
     */
  	context->config               = USART_INITSYNC_DEFAULT;
  	context->config.baudrate      = baudrate;
    context->config.master        = true;
    context->config.autoCsEnable  = false;
    context->config.clockMode     = usartClockMode3;
    context->config.msbf          = true;
    context->config.enable        = usartEnable;
    // USART_InitSync resets complete USART module
    // -> special register-settings (like "ROUTELOC") must
    //    be set afterwards!!!
    USART_InitSync(context->SPIx, &(context->config));

    /* pin routing */
  	switch(context->idx){
  	  case SPI_IDX0:
        USART0->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC0) | // US0_CLK       on location 0 = PA2
                            (USART_ROUTELOC0_TXLOC_LOC0)  | // US0_TX (MOSI) on location 0 = PA0
                            (USART_ROUTELOC0_RXLOC_LOC0);   // US0_RX (MISO) on location 0 = PA1
  	    break;

      case SPI_IDX1:
//        USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC3) | // US1_CLK       on location 3 = PA5
//                            (USART_ROUTELOC0_TXLOC_LOC3)  | // US1_TX (MOSI) on location 3 = PA3
//                            (USART_ROUTELOC0_RXLOC_LOC3);   // US1_RX (MISO) on location 3 = PA4
        USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC11) | // US1_CLK       on location 11 = PC8
                            (USART_ROUTELOC0_TXLOC_LOC11)  | // US1_TX (MOSI) on location 11 = PC6
                            (USART_ROUTELOC0_RXLOC_LOC11);   // US1_RX (MISO) on location 11 = PC7
        break;

      case SPI_IDX2:
        USART2->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC16) | // US2_CLK       on location 16 = PF5
                            (USART_ROUTELOC0_TXLOC_LOC16)  | // US2_TX (MOSI) on location 16 = PF3
                            (USART_ROUTELOC0_RXLOC_LOC16);   // US2_RX (MISO) on location 16 = PF4
        break;

      case SPI_IDX3:
        USART3->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC18) | // US3_CLK       on location 18 = PC2
                            (USART_ROUTELOC0_TXLOC_LOC18)  | // US3_TX (MOSI) on location 18 = PC0
                            (USART_ROUTELOC0_RXLOC_LOC18);   // US3_RX (MISO) on location 18 = PC1
        break;

      default:
        RODOS_ERROR("SPI index out of range");
        return -1;
  	}

  	context->SPIx->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  	/* configure GPIOs */
    GPIO_PinModeSet(context->GPIO_Port_SCK, context->GPIO_Pin_SCK, gpioModePushPull, 1);
    GPIO_PinModeSet(context->GPIO_Port_MOSI, context->GPIO_Pin_MOSI, gpioModePushPull, 0);
    GPIO_PinModeSet(context->GPIO_Port_MISO, context->GPIO_Pin_MISO, gpioModeInput, 0); // "out"-value sets filter on/off

  	context->initialized = true;
  	return 0;
}


int32_t HAL_SPI::writeRead(const void* sendBuf, size_t len, void* recBuf, size_t maxLen) {
	if(!context->initialized) return -1;
	
	uint8_t *pTx = (uint8_t*)sendBuf;
	uint8_t *pRx = (uint8_t*)recBuf;
	uint8_t rxVal = 0;
	size_t rxCnt = maxLen;

	/* transmit tx buffer and receive at the same time */
	unsigned int i = 0;
	for(i = 0; i < len; i++){
	    rxVal = USART_SpiTransfer(context->SPIx, *pTx);
	    pTx++;
	    if (rxCnt) {
	        *pRx = rxVal;
	        pRx++;
	        rxCnt--;
	    }
	}

	/* generate SCLK by sending dummy bytes to complete read request */
	while (rxCnt){
	    *pRx = USART_SpiTransfer(context->SPIx, 0);
	    pRx++;
	    rxCnt--;
	}

	return static_cast<int32_t>(maxLen);
}


int32_t HAL_SPI::write(const void* sendBuf, size_t len) {

	if(!context->initialized) return -1;
	
  uint8_t *pTx = (uint8_t*)sendBuf;

  /* transmit tx buffer */
  unsigned int i = 0;
  for(i = 0; i < len; i++){
      USART_SpiTransfer(context->SPIx, *pTx);
      pTx++;
  }

	return static_cast<int32_t>(len);
}


int32_t HAL_SPI::read(void* recBuf, size_t maxLen) {

	if(!context->initialized) return -1;
 
  uint8_t *pRx = (uint8_t*)recBuf;
  size_t rxCnt = maxLen;

  /* generate SCLK by sending dummy bytes */
  while (rxCnt){
      *pRx = USART_SpiTransfer(context->SPIx, 0);
      pRx++;
      rxCnt--;
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


