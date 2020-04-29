/*
 * Copyright (c) 2020 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Jorge Oliveira, Marcelo Alves
 */

#include "rodos.h"
#include "hal/hal_radio.h"

extern "C" {

	#include "hal-config.h"
	#include "hal_common.h"
    #include "vendor-headers.h"
	#include "bsp.h"
}

namespace RODOS {

// Used in the RAIL interrupt
HAL_RADIO* radioContext;

// Constructor
HAL_RADIO::HAL_RADIO()
{
	radioContext = this;
}

/**
 * Initialization of the device and radio protocol.
 *
 * @return RADIO_STATUS_OK in case of success. Otherwise, returns RADIO_STATUS_ERROR.
 */
RADIO_STATUS_TYPE HAL_RADIO::init (void)
{
	// Hardware initialization.
	halInit();
	CMU_ClockEnable(cmuClock_GPIO, true);
	BSP_Init(BSP_INIT_BCC);
	
	// Initialize RAIL with radio event callback.
	static RAIL_Config_t railCfg;
	railCfg.eventsCallback = &RAIL_callback;
    railHandle = RAIL_Init(&railCfg, NULL);
   
    if(railHandle == NULL) 
    	return RADIO_STATUS_ERROR; 
    
    // Initializes RAIL calibration.
    if(RAIL_ConfigCal(railHandle, RAIL_CAL_ALL) != RAIL_STATUS_NO_ERROR) 
		return RADIO_STATUS_ERROR;

    // Configures the channels supported by this device.
    RAIL_ConfigChannels(railHandle, channelConfigs[0], NULL);	

	// Configure radio events.
	RAIL_ConfigEvents(	railHandle,
						RAIL_EVENTS_ALL,
						( 	RAIL_EVENT_RX_PACKET_RECEIVED |
							RAIL_EVENT_TX_PACKET_SENT |
							RAIL_EVENT_TX_FIFO_ALMOST_EMPTY |
							RAIL_EVENT_RX_FIFO_ALMOST_FULL )
	);
    
	// Initialise the TX power settings currently used in the amplifier.
	RAIL_TxPowerConfig_t txPowerConfig;
	txPowerConfig.mode 		= RAIL_TX_POWER_MODE_2P4_HP;
	txPowerConfig.voltage  	= BSP_PA_VOLTAGE;
	txPowerConfig.rampTime 	= HAL_PA_RAMP;

  	if(RAIL_ConfigTxPower(railHandle, &txPowerConfig) != RAIL_STATUS_NO_ERROR) 
		return RADIO_STATUS_ERROR;
		
  	if(RAIL_SetTxPower(railHandle, HAL_PA_POWER) != RAIL_STATUS_NO_ERROR) 
		return RADIO_STATUS_ERROR;
		
  	// Replace default TX RAIL FIFO with custom buffer of size TX_FIFO_SIZE. 
  	// The RX FIFO is configured in RAILCb_SetupRxFifo callback).
  	uint16_t txFifoSize = RAIL_SetTxFifo(railHandle, txFifo, 0, TX_FIFO_SIZE);
  	
  	if(txFifoSize != TX_FIFO_SIZE) 
  		return RADIO_STATUS_ERROR;
		
    return RADIO_STATUS_OK;
}

/**
 * Copies sendBuff to tempBuff, which is used by the device for transmission.
 *
 * @return RADIO_STATUS_OK in case of success. Otherwise, returns RADIO_STATUS_ERROR.
 */
RADIO_STATUS_TYPE HAL_RADIO::write(const char* sendBuff, int len)
{
	if (len > MAX_NETWORK_MESSAGE_LENGTH) 
		return RADIO_STATUS_ERROR;

	memcpy(tempBuff, sendBuff, static_cast<size_t>(len));
	tempBuffLength = static_cast<uint16_t>(len);

	sendData = true;

	return RADIO_STATUS_OK;
}

/**
 * Copies sendBuff to tempBuff, which is used by the device for transmission.
 *
 * @return Number of bytes of the last data packet that was receive, or -1 if none were read.
 */
int HAL_RADIO::read(char* recBuf )
{
	if (dataReady == false) return -1;

	dataReady = false;

	if (lastDataPacketLength > MAX_NETWORK_MESSAGE_LENGTH)
		lastDataPacketLength = MAX_NETWORK_MESSAGE_LENGTH;
	
	// Only copy data after protocol header (first 3 bytes)	
	memcpy(recBuf, &lastDataPacket[3], static_cast<size_t>(lastDataPacketLength-3));

	return lastDataPacketLength-3;
}

bool HAL_RADIO::isWriteFinished()
{
	return true;
}

extern "C" {

/**
 * This function is called during the RAIL_Init process to set up the FIFO to use for received packets.
 *
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAILCb_SetupRxFifo(RAIL_Handle_t railHandle)
{
	uint16_t rxFifoSize = RX_FIFO_SIZE;
  	RAIL_Status_t status = RAIL_SetRxFifo(railHandle, &radioContext->rxFifo[0], &rxFifoSize);
  	
	if (rxFifoSize != RX_FIFO_SIZE) {
    	return RAIL_STATUS_INVALID_PARAMETER;
  		}
  	
	if (status == RAIL_STATUS_INVALID_STATE) {
    	return RAIL_STATUS_INVALID_STATE;
  	}

  	return status;
}

/**
 * Callback called upon failed assertion. 
 */
void RAILCb_AssertFailed([[gnu::unused]] RAIL_Handle_t railHandle, [[gnu::unused]] RAIL_AssertErrorCodes_t errorCode)
{
    // Reset the chip since an assert is a fatal error
    NVIC_SystemReset();
}

/**
 * Function called whenever a RAIL event occurs.
 * @note: only works for events initialized in HAL_RADIO::init().
 */
void RAIL_callback(RAIL_Handle_t railHandle, RAIL_Events_t events) 
{
	if(events & RAIL_EVENT_TX_PACKET_SENT)      
	{
		// Collection of TX packet timestamps. Used for Debug purposes.
		radioContext->txDetails.isAck = false;
		radioContext->txDetails.timeSent.timePosition = RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL;
		radioContext->txDetails.timeSent.totalPacketBytes = static_cast<uint32_t >(radioContext->packetLength);
		
		RAIL_GetTxPacketDetails(railHandle, &radioContext->txDetails);
		if (++radioContext->txTimestampCounter < 100)
			radioContext->tx_timestamp[radioContext->txTimestampCounter-1] = radioContext->txDetails.timeSent;
		
		// 	
		radioContext->packetSent = 1;		// A poll/data packet was sent.
		radioContext->sendData = false;		// A data packet was sent.
	}

	if(events & RAIL_EVENT_RX_PACKET_RECEIVED)
	{
		// the handling of received packets differs between Master and Slave Devices.
		radioContext->storeReceivedPackets(RAIL_GetRxFifoBytesAvailable(railHandle));
	}
}

} // end extern C

} // end namespace RODOS
