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
 * - read/write not "busy-waiting" -> interrupts & suspend
 * - DMA
 * - slave-mode
 * - use of autoCS-pin
 */

  /* driver features
   * - default polarity/phase = 0/0 -> clk is idle low, sample on rising edge
   * - TI mode not supported
   * - pin configuration with constructor
   * - baudrate and mode configuration with config()
   *
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

    GPIO_PIN GPIO_Pin_MISO;
    GPIO_PIN GPIO_Pin_MOSI;
    GPIO_PIN GPIO_Pin_SCK;
    GPIO_PIN GPIO_Pin_NSS;

  public:
    HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin);
    HW_HAL_SPI(SPI_IDX idx);

    HW_HAL_SPI() {
      // this constructor is called to init SPIcontextArray-objects
      // !!! don't set member variables -> this might overwrite values already set by constructor call of placement new
    }

    ~HW_HAL_SPI(){}

    void waitOnTransferReady();

    int32_t setBaudrate(uint32_t baudrate);
    int32_t enableClocks();
    int8_t getPinLoc(GPIO_PIN gpio, const uint8_t * lut); // return -1, if no "location" for "gpio" found, otherwise value between 0 ... 31

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

  GPIO_Pin_SCK    = sckPin;
	GPIO_Pin_MISO   = misoPin;
	GPIO_Pin_NSS    = nssPin;
	GPIO_Pin_MOSI   = mosiPin;
}

int32_t HW_HAL_SPI::setBaudrate(uint32_t baudrate){

    if (SPIx == NULL){return -1;}

    USART_BaudrateSyncSet(SPIx, 0, baudrate);

    this->baudrate = baudrate;

    return 0;
}

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

/* pin location tables
 * - to save memory only one LUT for rx-,tx- and clk-pin is used
 *   -> this is possible because the 3 pin-location-tables are shifted by just one value
 *   -> each pin-location-table has 32 items
 *   -> we combined all 3 tables to one LUT with 34 items and use different starting points for each pin search
 */
#define PIN_LOCATION_TABLE_SIZE   32

const uint8_t usart0_1PinLoc_LUT[PIN_LOCATION_TABLE_SIZE+2] = {
    GPIO_000, GPIO_001, GPIO_002, GPIO_003, GPIO_004, GPIO_005, // PA1 ... PA5
    GPIO_027, GPIO_028, GPIO_029, GPIO_030, GPIO_031, // PB11 ... PB15
    GPIO_038, GPIO_039, GPIO_040, GPIO_041, GPIO_042, GPIO_043, // PC06 ... PC11
    GPIO_057, GPIO_058, GPIO_059, GPIO_060, GPIO_061, GPIO_062, GPIO_063, // PD09 ... PD15
    GPIO_080, GPIO_081, GPIO_082, GPIO_083, GPIO_084, GPIO_085, GPIO_086, GPIO_087, // PF00 ... PF07
    GPIO_000, GPIO_001 // PA00, PA01 -> repeat first two elements to use LUT for all three pins
};

const uint8_t usart2PinLoc_LUT[PIN_LOCATION_TABLE_SIZE+2] = {
    GPIO_005, GPIO_006, GPIO_007, GPIO_008, GPIO_009, // PA5 ... PA9
    GPIO_128, GPIO_129, GPIO_130, GPIO_131, // PI0 ... PI3
    GPIO_022, GPIO_023, GPIO_024, GPIO_025, GPIO_026, // PB06 ... PB10
    GPIO_080, GPIO_081, GPIO_083, GPIO_084, GPIO_085, GPIO_086, GPIO_087, // PF00 ... PF07 (except PF02!)
    GPIO_088, GPIO_089, GPIO_090, GPIO_091, GPIO_092, GPIO_093, GPIO_094, GPIO_095, // PF08 ... PF15
    GPIO_160, GPIO_161, GPIO_162, // PK00 ... PK02
    GPIO_005, GPIO_006 // PA05 ... PA06 -> repeat first two elements to use LUT for all three pins
};

const uint8_t usart3PinLoc_LUT[PIN_LOCATION_TABLE_SIZE+2] = {
    GPIO_056, GPIO_057, GPIO_058, GPIO_059, GPIO_060, GPIO_061, GPIO_062, GPIO_063, // PD08 ... PD15
    GPIO_130, GPIO_131, // PI2 ... PI3
    GPIO_022, GPIO_023, GPIO_024, GPIO_025, GPIO_026, GPIO_027, // PB06 ... PB11
    GPIO_158, GPIO_159, // PJ14 ... PJ15
    GPIO_032, GPIO_033, GPIO_034, GPIO_035, GPIO_036, GPIO_037, // PC00 ... PC05
    GPIO_091, GPIO_092, GPIO_093, GPIO_094, GPIO_095, // PF11 ... PF15
    GPIO_160, GPIO_161, GPIO_162, // PK00 ... PK02
    GPIO_056, GPIO_057 // PD08 ... PD09 -> repeat first two elements to use LUT for all three pins
};


int8_t HW_HAL_SPI::getPinLoc(GPIO_PIN gpio, const uint8_t * lut){
    int i = 0;

    while(i < PIN_LOCATION_TABLE_SIZE){
        if(gpio == lut[i]){
            return i;
        }
        i++;
    }

    return -1;
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
 * @return 0 on success
 */
int32_t HAL_SPI::init(uint32_t baudrate, bool slave, bool tiMode) {       

    context->enableClocks();

    context->baudrate = baudrate;

  	context->config               = USART_INITSYNC_DEFAULT;
  	context->config.baudrate      = baudrate;
    context->config.master        = true;
    context->config.autoCsEnable  = false;
    context->config.clockMode     = usartClockMode0; // don't change this value -> use config()
    context->config.msbf          = true;
    context->config.enable        = usartEnable;
    // USART_InitSync resets complete USART module
    // -> special register-settings (like "ROUTELOC") must
    //    be set afterwards!!!
    USART_InitSync(context->SPIx, &(context->config));

    int8_t rxPinLoc = 0;
    int8_t txPinLoc = 0;
    int8_t clkPinLoc = 0;

    /* pin routing
     * - to save memory only one LUT for rx-,tx- and clk-pin is used
     *   -> this is possible because the 3 pin-location-tables are shifted by just one value
     *   -> each pin-location-table has 32 items
     *   -> we combined all 3 tables to one LUT with 34 items and use different starting points for each pin search
     */
  	switch(context->idx){
  	  case SPI_IDX0:
  	    txPinLoc = context->getPinLoc(context->GPIO_Pin_MOSI, &usart0_1PinLoc_LUT[0]);
  	    rxPinLoc = context->getPinLoc(context->GPIO_Pin_MISO, &usart0_1PinLoc_LUT[1]);
  	    clkPinLoc = context->getPinLoc(context->GPIO_Pin_SCK, &usart0_1PinLoc_LUT[2]);
  	    break;

      case SPI_IDX1:
        txPinLoc = context->getPinLoc(context->GPIO_Pin_MOSI, &usart0_1PinLoc_LUT[0]);
        rxPinLoc = context->getPinLoc(context->GPIO_Pin_MISO, &usart0_1PinLoc_LUT[1]);
        clkPinLoc = context->getPinLoc(context->GPIO_Pin_SCK, &usart0_1PinLoc_LUT[2]);
        break;

      case SPI_IDX2:
        txPinLoc = context->getPinLoc(context->GPIO_Pin_MOSI, &usart2PinLoc_LUT[0]);
        rxPinLoc = context->getPinLoc(context->GPIO_Pin_MISO, &usart2PinLoc_LUT[1]);
        clkPinLoc = context->getPinLoc(context->GPIO_Pin_SCK, &usart2PinLoc_LUT[2]);
        break;

      case SPI_IDX3:
        txPinLoc = context->getPinLoc(context->GPIO_Pin_MOSI, &usart3PinLoc_LUT[0]);
        rxPinLoc = context->getPinLoc(context->GPIO_Pin_MISO, &usart3PinLoc_LUT[1]);
        clkPinLoc = context->getPinLoc(context->GPIO_Pin_SCK, &usart3PinLoc_LUT[2]);
        break;

      default:
        RODOS_ERROR("SPI index out of range");
        return -1;
  	}

    if (rxPinLoc <0 || txPinLoc <0 || clkPinLoc < 0){
        RODOS_ERROR("SPI pin invalid");
        USART_Reset(context->SPIx);
        return -1;
    }

    context->SPIx->ROUTELOC0 =  (rxPinLoc << _USART_ROUTELOC0_RXLOC_SHIFT) |
                                (txPinLoc << _USART_ROUTELOC0_TXLOC_SHIFT) |
                                (clkPinLoc << _USART_ROUTELOC0_CLKLOC_SHIFT);

  	context->SPIx->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  	/* configure GPIOs */
    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_SCK), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_SCK), gpioModePushPull, 0);
    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_MOSI), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_MOSI), gpioModePushPull, 0);
    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_MISO), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_MISO), gpioModeInput, 0); // "out"-value sets filter on/off

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


int32_t HAL_SPI::config(SPI_PARAMETER_TYPE type, int32_t value){
    if(!context->initialized) return -1;

    switch (type){
    case SPI_PARAMETER_BAUDRATE:
        if ( (value > 0) && isWriteFinished() && isReadFinished()){
            USART_Enable(context->SPIx, usartDisable);
            context->setBaudrate(static_cast<uint32_t>(value));
            USART_Enable(context->SPIx, usartEnable);
            return 0;
        }
        return -1;

    case SPI_PARAMETER_MODE:
      // MODE:CPOL/CPHA  0:0/0   1:0/1   2:1/0   3:1/1
      if (value >= 0 && value <= 3){
          USART_Enable(context->SPIx, usartDisable);

          uint32_t cr1 = context->SPIx->CTRL;
          cr1 = cr1 & ~(_USART_CTRL_CLKPHA_MASK | _USART_CTRL_CLKPOL_MASK); // clear CPOL & CPHA
          uint32_t cpha = value & 1;
          uint32_t cpol = (value >> 1) & 1;
          cr1 = cr1 | (cpha << _USART_CTRL_CLKPHA_SHIFT) | (cpol << _USART_CTRL_CLKPOL_SHIFT);   // set CPHA & CPOL
          context->SPIx->CTRL = cr1;

          GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_SCK), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_SCK), gpioModePushPull, cpol);

          USART_Enable(context->SPIx, usartEnable);
          return 0;
      }else{
          return -1;
      }

    default:
        return -1;
    }
}

void HAL_SPI::reset(){
    if (context->SPIx == NULL) return;

    USART_Reset(context->SPIx);
    context->initialized = false;

    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_SCK), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_SCK), gpioModeDisabled, 0);
    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_MOSI), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_MOSI), gpioModeDisabled, 0);
    GPIO_PinModeSet(HW_HAL_GPIO::getEFR32Port(context->GPIO_Pin_MISO), HW_HAL_GPIO::getEFR32Pin(context->GPIO_Pin_MISO), gpioModeDisabled, 0);

    /* disable peripheral clock */
    switch(context->idx){
      case SPI_IDX0:
        CMU_ClockEnable(cmuClock_USART0, false);
        break;

      case SPI_IDX1:
        CMU_ClockEnable(cmuClock_USART1, false);
        break;

      case SPI_IDX2:
        CMU_ClockEnable(cmuClock_USART2, false);
        break;

      case SPI_IDX3:
        CMU_ClockEnable(cmuClock_USART3, false);
        break;

      default:
        RODOS_ERROR("SPI index out of range");
        return;
    }
}

int32_t HAL_SPI::status(SPI_STATUS_TYPE type) {
    if(!context->initialized) return -1;

    switch (type){
    case SPI_STATUS_BAUDRATE:
        //return static_cast<int32_t>(context->baudrate);
        return USART_BaudrateGet(context->SPIx);
    default:
        return -1;
    }
}

bool HAL_SPI::isWriteFinished(){
	return true;
}

bool HAL_SPI::isReadFinished(){
	return true;
}

}


