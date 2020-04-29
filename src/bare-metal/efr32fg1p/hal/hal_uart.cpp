/*
 * Copyright (c) 2019 Uni Wuerzburg, Wuerzburg

 * All rights reserved.
 *
 * @author Jorge Oliveira
 */

/* By driver enabled pins, "()"->also possible
 * - pins can be changed in HAL_UART::HAL_UART()
   -> Pins are used when selecting GPIO mode, and their LOC is used to route (check efr32fg1-datasheet page 129) 
   USART0 (VCOM) - 	Tx:  PA0 (LOC0)		Rx:  PA1 (LOC0)			VCOM Enable: PA5
            		CTS: PA2 (LOC30)	RTS: PA3 (LOC30)			          

   USART1 -  		Tx:  PB11 (LOC6)	Rx:  PB12 (LOC6)
            		CTS: PB13 (LOC4)	RTS: PB14 (LOC4)          
*/

#include "rodos.h"
#include "hal/hal_uart.h"
#include "hw_hal_gpio.h"

#include "vendor-headers.h"


#define UART_IDX_MIN        UART_IDX0
#define UART_IDX_MAX        UART_IDX1

namespace RODOS {

extern "C"
{
	void USART0_RX_IRQHandler();
	void USART0_TX_IRQHandler();
	void USART1_RX_IRQHandler();
	void USART1_TX_IRQHandler();

	void LDMA_IRQHandler();
}

class HW_HAL_UART {

	friend class ReceiveTrigger;
	friend class HAL_UART;
	friend void UART_putc_interrupt(HAL_UART* uart, uint8_t c);

	friend void USART0_RX_IRQHandler();
	friend void USART0_TX_IRQHandler();
	friend void USART1_RX_IRQHandler();
	friend void USART1_TX_IRQHandler();

	friend void LDMA_IRQHandler();

	UART_IDX idx;
    uint32_t hwFlowCtrl;
	uint32_t baudrate;
	HAL_UART* hal_uart;
	int uartRxError;


	USART_TypeDef *UARTx;

	GPIO_PIN tx;
	GPIO_PIN rx;
	GPIO_PIN rts;
	GPIO_PIN cts;

	bool isDMAEnabeld;
	volatile bool DMATransmitRunning;
	volatile size_t DMATransmitRunningSize;
	volatile bool DMAReceiveRunning;
	volatile size_t DMAReceiveRunningSize;

	LDMA_TransferCfg_t TxTransfer;
	LDMA_Descriptor_t TxDesc;
	LDMA_TransferCfg_t RxTransfer;
	LDMA_Descriptor_t RxDesc;

	size_t DMAMaxReceiveSize;

	BlockFifo<uint8_t,UART_BUF_SIZE> receiveBuffer;
	BlockFifo<uint8_t,UART_BUF_SIZE> transmitBuffer;

	void DMATransmitFinishedHandler();
	void DMAReceiveFinishedHandler();


	void DMAConfigure();
	void DMAStartTransfer(void* memoryBuffer, size_t len);
	void DMAStartReceive(void* memoryBuffer, size_t len);


	void SendTxBufWithDMA();
	void ReceiveIntoRxBufWithDMA();

	//There are 2 different interrupt vector groups: USART Reception, USART  Transmission
	void UART_RX_IRQHandler();		
	void UART_TX_IRQHandler();

	int init(uint32_t baudrate);
	void initMembers(HAL_UART* halUart, UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin, GPIO_PIN ctsPin);

	IRQn getUARTx_RX_IRQn();
	IRQn getUARTx_TX_IRQn();
	USART_TypeDef* getUARTx();
	CMU_Clock_TypeDef getUARTx_Clock();
	LDMA_PeripheralSignal_t getUARTx_TX_ldmaSignal();
	LDMA_PeripheralSignal_t getUARTx_RX_ldmaSignal();

	void putcharInterrupt(uint8_t c);
};


HW_HAL_UART UART_contextArray[UART_IDX_MAX+1];


HAL_UART::HAL_UART(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin, GPIO_PIN ctsPin) 
{
    if ((uartIdx < UART_IDX_MIN) || (uartIdx > UART_IDX_MAX)) {
        context = nullptr;
        return;
    }

	context = &UART_contextArray[uartIdx];
	context->initMembers(this, uartIdx, txPin, rxPin, rtsPin, ctsPin);
}


HAL_UART::HAL_UART(UART_IDX uartIdx)
{
    if ((uartIdx < UART_IDX_MIN) || (uartIdx > UART_IDX_MAX)) {
      context = nullptr;
      return;
    }

	context = &UART_contextArray[uartIdx];

	switch (uartIdx){
	case UART_IDX0: //                tx-PA0    rx-PA1    rts-PA3  cts-PA2
	    context->initMembers(this, uartIdx, GPIO_000, GPIO_001, GPIO_003, GPIO_002);
	    break;

    case UART_IDX1: //                tx-PB11    rx-PB12    rts-PB14   cts-PB13 
        context->initMembers(this, uartIdx, GPIO_012, GPIO_013, GPIO_015, GPIO_014);
        break;

    default: break;
	}
}



extern "C"
{

void USART0_RX_IRQHandler() 
{
	UART_contextArray[UART_IDX0].UART_RX_IRQHandler();
	NVIC_ClearPendingIRQ(USART0_RX_IRQn);
}

void USART1_RX_IRQHandler() 
{
	UART_contextArray[UART_IDX1].UART_RX_IRQHandler();
	NVIC_ClearPendingIRQ(USART1_RX_IRQn);
}

void USART0_TX_IRQHandler() 
{
	UART_contextArray[UART_IDX0].UART_TX_IRQHandler();
	NVIC_ClearPendingIRQ(USART0_TX_IRQn);
}

void USART1_TX_IRQHandler() 
{
	UART_contextArray[UART_IDX1].UART_TX_IRQHandler();
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);
}

void LDMA_IRQHandler(void)
{
 	uint32_t ch = 0;
 	uint32_t mask = 0;
  	// Get all pending and enabled interrupts.
  	uint32_t pending = LDMA_IntGetEnabled();
  	// Iterate over all LDMA channels.
  	for (ch = 0; ch < DMA_CHAN_COUNT; ch++)
  	{
    	mask = 0x1u << ch;
    	if (pending & mask)
   	 	{
      		// Clear interrupt flag.
      		LDMA->IFC = mask;
      		// 
      		switch(ch)
      		{
      		 	case 0:
      		 		UART_contextArray[UART_IDX0].DMATransmitFinishedHandler();
      		 		break;
      		 	case 1:
					UART_contextArray[UART_IDX1].DMATransmitFinishedHandler();
					break;
      		 	case 2:
					UART_contextArray[UART_IDX0].DMAReceiveFinishedHandler();
					break;
      		 	case 3:
					UART_contextArray[UART_IDX1].DMAReceiveFinishedHandler();
					break;
				default:
					break;
      		}
		}
  	}
}

} // end extern "C"


/*
 * USART
 * - all USART will be initialized in 8N1 mode
 */
int32_t HAL_UART::init(uint32_t iBaudrate) {

	if (context == nullptr) {return -1;}

	return context->init(iBaudrate);
}


int32_t HAL_UART::config(UART_PARAMETER_TYPE type, int32_t paramVal) {

	if (context == nullptr) {return -1;}

	USART_TypeDef* usart = context->UARTx;
	USART_InitAsync_TypeDef Uis = USART_INITASYNC_DEFAULT;

	Uis.baudrate = context->baudrate;

	switch (type) {
		case UART_PARAMETER_BAUDRATE:
			if (paramVal > 0)
			{
				Uis.baudrate = static_cast<uint32_t >(paramVal);
				context->baudrate = static_cast<uint32_t >(paramVal);
			}
			else {return -1;}
			break;

		case UART_PARAMETER_HW_FLOW_CONTROL:
			/* if rts and cts are set to GPIO_INVALID HW flow control is not available */
			if (paramVal > 0 && context->rts != GPIO_INVALID){ // enable HW Flow Control
				context->hwFlowCtrl |= USART_ROUTEPEN_RTSPEN | USART_ROUTEPEN_CTSPEN;
				HW_HAL_GPIO::configurePin(context->cts, gpioModeInput, 0);
				HW_HAL_GPIO::configurePin(context->rts, gpioModePushPull, 1);
			} else if(context->rts != GPIO_INVALID){  // disable HW Flow Control
				context->hwFlowCtrl &= ~(USART_ROUTEPEN_RTSPEN);
				context->hwFlowCtrl &= ~(USART_ROUTEPEN_CTSPEN);

				HW_HAL_GPIO::resetPin(context->rts);
				HW_HAL_GPIO::resetPin(context->cts);
			}
			break;  // end case UART_PARAMETER_HW_FLOW_CONTROL

		case UART_PARAMETER_ENABLE_DMA:
		    context->DMAConfigure();
		    if((unsigned int)paramVal > UART_BUF_SIZE)
		    	context->DMAMaxReceiveSize = UART_BUF_SIZE;
		    else
				context->DMAMaxReceiveSize = static_cast<size_t>(paramVal);
		    context->isDMAEnabeld = true;
			//context->ReceiveIntoRxBufWithDMA();
		    return 0;

		default: return -1;
	}

	usart->ROUTEPEN = context->hwFlowCtrl; 

	USART_Enable(usart, usartDisable);
	USART_InitAsync(usart, &Uis);
	USART_Enable(usart, usartEnable);

	return 0;
	
}


void HAL_UART::reset()
{
	if (context == nullptr) {return;}

	USART_TypeDef *usart = context->UARTx;

	// Interrupt UART disable
	NVIC_DisableIRQ(context->getUARTx_RX_IRQn());
	NVIC_DisableIRQ(context->getUARTx_TX_IRQn());

	// reset interface
	USART_Reset(usart);

	HW_HAL_GPIO::resetPin(context->rx);
	HW_HAL_GPIO::resetPin(context->tx);

	if (((context->hwFlowCtrl & _USART_ROUTEPEN_CTSPEN_MASK) && (context->hwFlowCtrl & _USART_ROUTEPEN_RTSPEN_MASK)) ||
	    (context->hwFlowCtrl & _USART_ROUTEPEN_RTSPEN_MASK)){
        if (context->rts != GPIO_INVALID) { HW_HAL_GPIO::resetPin(context->rts); }
	}

	if (((context->hwFlowCtrl & _USART_ROUTEPEN_CTSPEN_MASK) && (context->hwFlowCtrl & _USART_ROUTEPEN_RTSPEN_MASK)) ||
	    (context->hwFlowCtrl & _USART_ROUTEPEN_CTSPEN_MASK)){
        if (context->cts != GPIO_INVALID) { HW_HAL_GPIO::resetPin(context->cts); }
	}
}


size_t HAL_UART::read(void* buf, size_t size)
{
    if (context == nullptr) {return 0;}
    if(size <=0) return 0;

    size_t readCnt = 0;
    size_t i = 0;

    uint8_t* p = context->receiveBuffer.getBufferToRead(readCnt);

    if (p) {
        if (readCnt > size) {readCnt = size;}

        for (i = 0; i < readCnt; i++ ) {
            static_cast<uint8_t*>(buf)[i] = p[i];
        }
        context->receiveBuffer.readConcluded(readCnt);

    	if(context->isDMAEnabeld)
		{
			if (!context->DMAReceiveRunning)
			{
				context->ReceiveIntoRxBufWithDMA();
			}
		}
		else
		{
  			USART_IntEnable(context->UARTx, USART_IEN_RXDATAV);
		}

        return readCnt;
    } else {
        return 0;
    }
}


size_t HAL_UART::write(const void* buf, size_t size)
{
    if (context == nullptr) {return 0;}
    if(size <= 0) return 0;

    size_t i = 0;

    size_t spaceinbuffer;

    uint8_t* p = context->transmitBuffer.getBufferToWrite(spaceinbuffer);

    if(p)
    {
        if(spaceinbuffer < size)
        {
            size=spaceinbuffer;
        }

        for (i=0;i<size;i++)
        {
            p[i]=static_cast<const uint8_t*>(buf)[i];
        }
        context->transmitBuffer.writeConcluded(size);
        USART_IntDisable(context->UARTx, USART_IEN_TXC);
        if(context->isDMAEnabeld)
        {
            if(!context->DMATransmitRunning){
                context->SendTxBufWithDMA();
            }
        }
        else
        {
            USART_IntEnable(context->UARTx, USART_IEN_TXBL);
        }
        return size;
    }
    return 0;
}


int16_t HAL_UART::getcharNoWait()
{
    if (context == nullptr) {return -1;}

	uint8_t c = 0;
	bool dataAvailible = context->receiveBuffer.get(c);

	if(context->isDMAEnabeld)
	{
		if (!context->DMAReceiveRunning)
		{
			context->ReceiveIntoRxBufWithDMA();
		}
		
	}
	else
	{
  		USART_IntEnable(context->UARTx, USART_IEN_RXDATAV);
	}


	if (dataAvailible)
		return (int)c;
	return -1;
}


int16_t HAL_UART::putcharNoWait(uint8_t c)
{
    if (context == nullptr) {return -1;}

	if(context->transmitBuffer.put(c))
	{
		USART_IntDisable(context->UARTx, USART_IEN_TXC);
		if(context->isDMAEnabeld)
		{
			if(!context->DMATransmitRunning){
				context->SendTxBufWithDMA();
			}
		}
		else
		{
			USART_IntEnable(context->UARTx, USART_IEN_TXBL);
		}
		return c & 0xFF;
	}
	return -1;
}


int32_t HAL_UART::status(UART_STATUS_TYPE type)
{
    if (context == nullptr) {return -1;}

	switch (type)
	{
		case UART_STATUS_RX_BUF_LEVEL:
			return static_cast<int32_t>(context->receiveBuffer.getElementCount());

		case UART_STATUS_RX_ERROR:
			int temp;
			temp = context->uartRxError;
			context->uartRxError = 0;
			return temp;

		// case UART_STATUS_TX_COMPLETE:
		//     return (USART_StatusGet(context->UARTx) & USART_STATUS_TXC);
		//...
		default: return -1;
	}
}


bool HAL_UART::isWriteFinished() 
{
    if (context == nullptr) {return true;} // false would create an infinite loop
    return context->transmitBuffer.isEmpty() && (USART_StatusGet(context->UARTx) & USART_STATUS_TXC);
}


bool HAL_UART::isDataReady() 
{
    if (context == nullptr) {return false;}
    return !context->receiveBuffer.isEmpty();
}





/****************** HW_HAL_UART *******************/

void HW_HAL_UART::initMembers(HAL_UART* halUart, UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin, GPIO_PIN ctsPin)
{
 	idx = uartIdx;
    baudrate = 115200;
    hal_uart = halUart;
    isDMAEnabeld = false;
    DMATransmitRunning = false;
    DMAReceiveRunning = false;
    UARTx = getUARTx();
    hwFlowCtrl = UARTx->ROUTEPEN; //contains rts and cts bits
    tx = txPin;
    rx = rxPin;
    rts = rtsPin;
    cts = ctsPin;
}


/*************DMA**************/

void HW_HAL_UART::DMAConfigure() 
{

	// Initialize the LDMA
  	LDMA_Init_t init;
  	init.ldmaInitCtrlNumFixed     = _LDMA_CTRL_NUMFIXED_DEFAULT; 	// Fixed priority arbitration. 
  	init.ldmaInitCtrlSyncPrsClrEn = 0;                           	// No PRS Synctrig clear enable
  	init.ldmaInitCtrlSyncPrsSetEn = 0;                          	// No PRS Synctrig set enable.  
  	init.ldmaInitIrqPriority      = 3;                          	// IRQ priority level 3.        
  	LDMA_Init(&init);

	// Configure Tx DMA Descriptor (Single memory to peripheral)
    TxDesc.xfer.structType   = ldmaCtrlStructTypeXfer;
    TxDesc.xfer.structReq    = 0;
    TxDesc.xfer.byteSwap     = 0;
    TxDesc.xfer.blockSize    = ldmaCtrlBlockSizeUnit1;
    TxDesc.xfer.doneIfs      = 1;
    TxDesc.xfer.reqMode      = ldmaCtrlReqModeBlock;
    TxDesc.xfer.decLoopCnt   = 0;
    TxDesc.xfer.ignoreSrec   = 0;
    TxDesc.xfer.srcInc       = ldmaCtrlSrcIncOne;
    TxDesc.xfer.size         = ldmaCtrlSizeByte;
    TxDesc.xfer.dstInc       = ldmaCtrlDstIncNone;
    TxDesc.xfer.srcAddrMode  = ldmaCtrlSrcAddrModeAbs;
    TxDesc.xfer.dstAddrMode  = ldmaCtrlDstAddrModeAbs;
    TxDesc.xfer.dstAddr      = (uint32_t)(&getUARTx()->TXDATA);	// Destination address
    TxDesc.xfer.linkMode     = 0;
    TxDesc.xfer.link         = 0;
    TxDesc.xfer.linkAddr     = 0;

	// Configure Rx DMA Descriptor (Single peripheral to memory)
    RxDesc.xfer.structType   = ldmaCtrlStructTypeXfer;             
    RxDesc.xfer.structReq    = 0;
    RxDesc.xfer.byteSwap     = 0;
    RxDesc.xfer.blockSize    = ldmaCtrlBlockSizeUnit1;
    RxDesc.xfer.doneIfs      = 1;
    RxDesc.xfer.reqMode      = ldmaCtrlReqModeBlock;
    RxDesc.xfer.decLoopCnt   = 0;
    RxDesc.xfer.ignoreSrec   = 0;
    RxDesc.xfer.srcInc       = ldmaCtrlSrcIncNone;
    RxDesc.xfer.size         = ldmaCtrlSizeByte;
    RxDesc.xfer.dstInc       = ldmaCtrlDstIncOne;
    RxDesc.xfer.srcAddrMode  = ldmaCtrlSrcAddrModeAbs;
    RxDesc.xfer.dstAddrMode  = ldmaCtrlDstAddrModeAbs;
    RxDesc.xfer.srcAddr      = (uint32_t)(&getUARTx()->RXDATA); // Source Address
    RxDesc.xfer.linkMode     = 0;
    RxDesc.xfer.link         = 0;
    RxDesc.xfer.linkAddr     = 0;
}

void HW_HAL_UART::SendTxBufWithDMA() 
{
	size_t len;
	uint8_t*  p = transmitBuffer.getBufferToRead(len);

	if(p)
	{
		DMATransmitRunningSize = len;
		DMATransmitRunning = true;
		DMAStartTransfer(p, len);
	}
}

void HW_HAL_UART::DMAStartTransfer(void* memoryBuffer,size_t len)
{
	
	// Complete configuration of Tx DMA Descriptor
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    TxDesc.xfer.xferCnt      = (len) - 1;						// Number of byte transfers
#pragma GCC diagnostic pop
	TxDesc.xfer.srcAddr      = (uint32_t)(memoryBuffer);		// Source address
	// DMA transfer configuration
  	TxTransfer = LDMA_TRANSFER_CFG_PERIPHERAL(getUARTx_TX_ldmaSignal());
  	// Start DMA transfer
  	LDMA_StartTransfer(idx, &TxTransfer, &TxDesc);				// idx = channel number
}

void HW_HAL_UART::DMATransmitFinishedHandler()
{

    size_t bytesNotTransfered = LDMA_TransferRemainingCount(idx); 	// idx = channel number
	size_t bytesTransfered = DMATransmitRunningSize - bytesNotTransfered;

	transmitBuffer.readConcluded(bytesTransfered);

	if(transmitBuffer.isEmpty())
	{ 
		DMATransmitRunning = false;
		USART_IntEnable(UARTx, USART_IEN_TXC);	// Enabled to check when the Transmission is complete (shift resgister is empty)
	}
	else
	{
		SendTxBufWithDMA();
	}
}

void HW_HAL_UART::ReceiveIntoRxBufWithDMA() {

	size_t len;
	uint8_t* p = receiveBuffer.getBufferToWrite(len);

	if(p)
	{
		if(len > DMAMaxReceiveSize) 
			len = DMAMaxReceiveSize;
		DMAReceiveRunningSize = 1;
		DMAReceiveRunning = true;
		DMAStartReceive(p, 1);
	}
	else 
	{
		DMAReceiveRunning = false; //Buffer Overflow, disable Receive until next read
		uartRxError++;
	}
}


void HW_HAL_UART::DMAStartReceive(void* memoryBuffer, size_t len)
{

	// Complete configuration of Rx DMA Descriptor
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    RxDesc.xfer.xferCnt      = ((len) - 1);						// Number of byte transfers
#pragma GCC diagnostic pop
	RxDesc.xfer.dstAddr      = (uint32_t)(memoryBuffer);		// Destination address
	// DMA transfer configuration
  	RxTransfer = LDMA_TRANSFER_CFG_PERIPHERAL(getUARTx_RX_ldmaSignal());
  	// Start DMA transfer
  	LDMA_StartTransfer(idx + 2, &RxTransfer, &RxDesc);			// idx + 2 = channel number
}

void HW_HAL_UART::DMAReceiveFinishedHandler() 
{

	size_t bytesTransfered = DMAReceiveRunningSize - LDMA_TransferRemainingCount(idx + 2);	// idx + 2 = channel number

	receiveBuffer.writeConcluded(bytesTransfered);

	if(bytesTransfered > 0)
	{
		hal_uart->upCallDataReady();
	}

	ReceiveIntoRxBufWithDMA();
}



void HW_HAL_UART::UART_RX_IRQHandler()
{
	uint8_t c;

	//test overrun error and when DMA is enabled frame error, noise error and overrun error 
/*	if (USART_GetFlagStatus(UARTx,USART_FLAG_ORE) ||
    	(isDMAEnabeld && (USART_GetITStatus(UARTx,USART_IT_FE) || USART_GetITStatus(UARTx,USART_IT_NE) || USART_GetITStatus(UARTx,USART_IT_ORE_ER))))
    {
        USART_ReceiveData(UARTx);
        uartRxError++;
    }
    else */
	if(USART_IntGet(UARTx) & USART_IF_RXDATAV)
	{
		c = USART_RxDataGet(UARTx) & 0xFF;
        if(!receiveBuffer.put(c))
            uartRxError++; //FIFO OVerflow
        USART_IntDisable(UARTx, USART_IEN_RXDATAV);
        hal_uart->upCallDataReady();
	}
}

void HW_HAL_UART::UART_TX_IRQHandler()
{
	uint8_t c;

	if(USART_IntGet(UARTx) & USART_IF_TXC)				// Transmission is complete (shift resgister is empty)
	{
		USART_IntDisable(UARTx, USART_IEN_TXC);
        USART_IntClear(UARTx, USART_IntGet(UARTx));		// Clear flags 

       	hal_uart->upCallWriteFinished();
    }
	else
	{
		if (USART_IntGet(UARTx) & USART_IF_TXBL)		// Ready to send (Empty TX buffer)
		{
			if(transmitBuffer.get(c))
			{
            	USART_Tx(UARTx,c);
        	}
        	else
        	{
            	// no more data, disable TXBL(transmitter idle/empty) and enable TXC(transmission complete)
            	USART_IntDisable(UARTx, USART_IEN_TXBL);
            	USART_IntClear(UARTx, USART_IntGet(UARTx));		// Clear flags 

            	USART_IntEnable(UARTx, USART_IEN_TXC);
        	}
		}	
	} 
}

IRQn HW_HAL_UART::getUARTx_RX_IRQn() 
{
    switch(idx){
   	case UART_IDX0: return USART0_RX_IRQn;
    case UART_IDX1: return USART1_RX_IRQn;
    default: return USART0_RX_IRQn;
    }
}

IRQn HW_HAL_UART::getUARTx_TX_IRQn() 
{
    switch(idx){
   	case UART_IDX0: return USART0_TX_IRQn;
    case UART_IDX1: return USART1_TX_IRQn;
    default: return USART0_TX_IRQn;
    }
}

USART_TypeDef* HW_HAL_UART::getUARTx() 
{
    switch(idx){
    case UART_IDX0: return USART0;
    case UART_IDX1: return USART1;
    default: return NULL;
    }
}

CMU_Clock_TypeDef HW_HAL_UART::getUARTx_Clock()
{
	switch(idx){
	case UART_IDX0: return cmuClock_USART0;
    case UART_IDX1: return cmuClock_USART1;
    default: return cmuClock_USART0;
	}
}

LDMA_PeripheralSignal_t HW_HAL_UART::getUARTx_TX_ldmaSignal() 
{
    switch(idx){
   	case UART_IDX0: return ldmaPeripheralSignal_USART0_TXEMPTY;
    case UART_IDX1: return ldmaPeripheralSignal_USART1_TXEMPTY;
    default: return ldmaPeripheralSignal_USART0_TXEMPTY;
    }
}

LDMA_PeripheralSignal_t HW_HAL_UART::getUARTx_RX_ldmaSignal() 
{
    switch(idx){
   	case UART_IDX0: return ldmaPeripheralSignal_USART0_RXDATAV;
    case UART_IDX1: return ldmaPeripheralSignal_USART1_RXDATAV;
    default: return ldmaPeripheralSignal_USART0_RXDATAV;
    }
}


int HW_HAL_UART::init(uint32_t baudrate)
{
    if ((idx < UART_IDX_MIN) || (idx > UART_IDX_MAX)) {return -1;}

    this->baudrate = baudrate;

    USART_InitAsync_TypeDef Uis = USART_INITASYNC_DEFAULT;	

    CMU_ClockEnable(getUARTx_Clock(), true); // Enable oscillator to USART module
    
    USART_Reset(UARTx);	// Release reset

	// set pin modes for USART TX and RX pins and VCOM enable
	HW_HAL_GPIO::configurePin(this->rx, gpioModeInput, 0);
	HW_HAL_GPIO::configurePin(this->tx, gpioModePushPull, 1);
	if(idx == UART_IDX0) 				// Enable VCOM - USART0 only!!
		GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 1);  

    Uis.baudrate = baudrate;  			// set baudrate
	USART_InitAsync(UARTx, &Uis);		// init USART
	
	// Route pins
	if(idx == UART_IDX0)
		UARTx->ROUTELOC0 = USART_ROUTELOC0_RXLOC_LOC0 | USART_ROUTELOC0_TXLOC_LOC0;	// !!
	else
		UARTx->ROUTELOC0 = USART_ROUTELOC0_RXLOC_LOC6 | USART_ROUTELOC0_TXLOC_LOC6;	// !!					
  	
  	UARTx->ROUTEPEN |= USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  	// Enable USART Interrupts
  	NVIC_EnableIRQ(getUARTx_RX_IRQn());
  	NVIC_EnableIRQ(getUARTx_TX_IRQn());  
  	//USART_IntEnable(UARTx, USART_IEN_RXDATAV);
    return 0;
}

void HW_HAL_UART::putcharInterrupt(uint8_t c)
{
	while(!(USART_StatusGet(UARTx) & USART_STATUS_TXC)) {}
	USART_Tx(UARTx,c);
}


void UART_putc_interrupt(HAL_UART* uart, uint8_t c)
{
	for(int i=UART_IDX_MIN;i<=UART_IDX_MAX;i++){
		if(UART_contextArray[i].hal_uart == uart){
			UART_contextArray[i].putcharInterrupt(c);
			return;
		}
	}
}

}
