/*
 * Copyright (c) 2020 Uni Wuerzburg, Wuerzburg

 * All rights reserved.
 *
 * @author Jorge Oliveira, Marcelo Alves
 */

#include "rodos.h"
#include "hal/hal_radio_mstrslv.h"

extern "C" {
#include "rail.h"
#include "rail_types.h"
#include "rail_config.h"
#include "hal-config.h"
#include "hal_common.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "bsp.h"
#include "gpiointerrupt.h"
}

// Finite State Machine of Master devices.
enum MASTER_STATE{
	MASTER_STATE_POLL,
	MASTER_STATE_LISTEN,
	MASTER_STATE_DATA
};

// Finite State Machine of Slave devices.
enum SLAVE_STATE{
	SLAVE_STATE_LISTEN,
	SLAVE_STATE_DATA
};

namespace RODOS {

/****************MASTER****************/

/**
 * State switching of Master’s FSM.
 *
 * @return 1 if a state switch occurs, otherwise returns 0.
 */
bool HAL_RADIO_MASTER::switchState()
{
	switch(currentState) {	
		case MASTER_STATE_POLL:
			if(packetSent) 
			{
				packetSent  = 0;
				currentState = MASTER_STATE_LISTEN;
				return true; 
			}
			break;
	
		case MASTER_STATE_LISTEN:
			if(RAIL_IsTimerExpired(railHandle)) 
			{
				if (sendData == false)
				{
					tx_time += DATA_TIMESLOT_INTERVAL;
					currentState = MASTER_STATE_POLL;
					return true;
				} 
					
				currentState = MASTER_STATE_DATA;
				return true; 
			}
			break;
		case MASTER_STATE_DATA:
			if(packetSent) 
			{
				packetSent  = 0;
				currentState = MASTER_STATE_POLL;
				return true; 
			}
			break;
	}
	return false;  
}

/**
 * State execution of Master’s FSM.
 */
void HAL_RADIO_MASTER::executeState()
{
	switch(currentState) 
	{
		case MASTER_STATE_POLL:		poll(); 	break;
		case MASTER_STATE_LISTEN:	listen();	break;
		case MASTER_STATE_DATA:		data();		break;
	}
}

/**
 * Execution of state "POLL" of Master’s FSM.
 */
void HAL_RADIO_MASTER::poll()
{
	// Initialize TX FIFO with header information of the poll packet.
	initPoll();

	// Schedule transmission of poll at the instant "tx_time", which corresponds to the beginning of the first Slave’s timeslot.
	RAIL_ScheduleTxConfig_t scheduleTxConfig;

	scheduleTxConfig.when		= tx_time;
	scheduleTxConfig.mode		= RAIL_TIME_ABSOLUTE;
	scheduleTxConfig.txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX;

	RAIL_Status_t status = RAIL_StartScheduledTx(railHandle, pollChannel, RAIL_TX_OPTIONS_DEFAULT, &scheduleTxConfig, NULL);

	// Failure to schedule poll TX properly.
	if (status != RAIL_STATUS_NO_ERROR)
	{
		// Ignore the failure, continue as if it was correctly scheduled and sent.
		packetSent = 1;		 
	}
	
	// Next poll/data transmission will only happen after the Slave’s timeslots have elapsed.
	tx_time += DATA_TIMESLOT_INTERVAL*numberOfSlaves + POLL_TIMESLOT_INTERVAL;
}

/**
 * Execution of state "LISTEN" of Master’s FSM.
 */
void HAL_RADIO_MASTER::listen()
{	
	// Configure automatic state transitions in RX state.
	RAIL_StateTransitions_t transitions = {RAIL_RF_STATE_RX, RAIL_RF_STATE_RX};
	RAIL_SetRxTransitions(railHandle, &transitions);

	RAIL_StartRx(railHandle, dataChannel, NULL);
	
	RAIL_SetTimer(railHandle, tx_time-LISTEN_INTERVAL, RAIL_TIME_ABSOLUTE, NULL); 
}

/**
 * Execution of state "SEND DATA" of Master’s FSM.
 */
void HAL_RADIO_MASTER::data()
{	
	// Initialize TX FIFO with header information of the data packet.
	initMessages(tempBuffLength);
	
	// Append data contained in tempBuff to the TX FIFO. 
	RAIL_WriteTxFifo(railHandle, tempBuff, tempBuffLength, false);

	// Schedule transmission of data at the instant "tx_time", which corresponds to the beginning of the Master’s timeslot.
	RAIL_ScheduleTxConfig_t scheduleTxConfig;
	scheduleTxConfig.when		= tx_time;
	scheduleTxConfig.mode		= RAIL_TIME_ABSOLUTE;
	scheduleTxConfig.txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX;

	// Note: Master must send data in the poll channel, because Slaves only listen in that channel.
	RAIL_Status_t status = RAIL_StartScheduledTx(railHandle, pollChannel, RAIL_TX_OPTIONS_DEFAULT, &scheduleTxConfig, NULL);

	// Failure to schedule data TX properly. 
	if (status != RAIL_STATUS_NO_ERROR)
	{
		// Ignore the failure, continue as if it was correctly scheduled and sent.
		packetSent = 1;	
	}
	
	// Next poll transmission will only happen after the Master’ timeslots have elapsed.
	tx_time += DATA_TIMESLOT_INTERVAL;
}

/**
 * Used to change protocol parameters during runtime.
 *
 * @return TRUE if valid parameters were introduced, otherwise returns FALSE.
 */
bool HAL_RADIO_MASTER::config(RADIO_PARAMETER_TYPE type, int paramVal)
{
	switch (type)
	{
		case POLL_CHANNEL:
			if (paramVal < channelConfigs[0]->configs->channelNumberStart || paramVal > channelConfigs[0]->configs->channelNumberEnd)
				return false;
			else
				pollChannel = paramVal;
			break;
		case DATA_CHANNEL:
			if (paramVal < channelConfigs[0]->configs->channelNumberStart || paramVal > channelConfigs[0]->configs->channelNumberEnd)
				return false;
			else
				dataChannel = paramVal;
			break;
		case NUMBER_OF_SLAVES:
			if (paramVal < 0 || paramVal > MAX_NUMBER_SLAVES )
				return false;
			else
				numberOfSlaves = paramVal;
			break;

		default: return false;
	}
	return true;
}

/**
 *  Called by RAIL event interrupt when packet reception occurs.
 */
void HAL_RADIO_MASTER::storeReceivedPackets(uint16_t bytesAvailable) 
{	
	static uint16_t rxReceived = 0;	// Number of bytes collected from RX FIFO.
	static uint16_t rxExpected = 0;	// Total number of bytes of the packet, according to header information.
	
	// If the previous packet was complete received with success, rxExpected will be 0.
	if(rxExpected == 0) 
	{
		rxReceived = RAIL_ReadRxFifo(railHandle, rxPtr, bytesAvailable);
		rxExpected = (rxPtr[0] << 8) + rxPtr[1] + PACKET_HEADER_LEN;
		
		// Packet was completely received.
		if(rxExpected == rxReceived)
		{
		 	rxExpected = 0;
		 	memcpy(lastDataPacket, rxPtr, rxReceived);
			lastDataPacketLength = rxReceived;
			dataReady = true;
		}
		
		// Collection of RX packet timestamps. Used for Debug purposes.
		rxDetails.timeReceived.totalPacketBytes = rxReceived;
		rxDetails.timeReceived.timePosition = RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL;
		RAIL_GetRxPacketDetails(railHandle, RAIL_RX_PACKET_HANDLE_NEWEST, &rxDetails);
		if (++rxTimestampCounter < 100)
			rx_timestamp[rxTimestampCounter-1] = rxDetails.timeReceived;
	}
	else  // If the previous packet was not completely received yet, concatenation is needed.
	{
		uint16_t _rxReceived = rxReceived;
		uint16_t _rxExpected = rxExpected;
		rxReceived += RAIL_ReadRxFifo(railHandle, rxPtr + _rxReceived, _rxExpected - _rxReceived);
	}
}

/**
 * Reset TX FIFO and initialize it with header bytes of POLL packet information.
 */
void HAL_RADIO_MASTER::initPoll()
{
	// The first 2 bytes contain lenght information (RAIL frame header).
	// The third byte indicates that it is a POLL packet.
	// The rest contain information about the slave’s timeslots.
	uint8_t pollPacket[3+MAX_NUMBER_SLAVES];	

	packetLength = 3+numberOfSlaves;	// This line is only used for Debug purposes.
	
	// Packet length information is stored MSB first.
	// It uses RAIL’s Frame Variable Length feature, using 12 bits to store length information along 2 bytes.
	// Visual guide of bit order:	x x x x 12 11 10 9 | 8 7 6 5  4 3 2 1
	// 12 bits allow for a maximum size of 4096 bytes, but the protocol limit is specified in the MAX_PACKET_LENGTH macro.
	// This specifications are stored in the *.isc file, generated by Simplicity Studio.
	pollPacket[0] = (3+numberOfSlaves - PACKET_HEADER_LEN) >> 8;
	pollPacket[1] = (3+numberOfSlaves - PACKET_HEADER_LEN) & 0xFF;
	
	// Note: this following byte is not part of the frame header, according to RAIL nomenclature.
	pollPacket[2] = 0xFF;	// the "0xFF" value in pollPacket[2] means the packet is a POLL packet.

	// Each slave is assigned a timeslot. 
	// Each byte of the POLL packet after position 3 corresponds to a single Slave ID, starting from ID=0.
	// for example, Slave 4 will read position [4+3] of the POLL to determine when (and if) his timeslot starts.
	for (int i = 0; i < numberOfSlaves; i++)
	{
		pollPacket[i+3] = i;
	}

	RAIL_WriteTxFifo(railHandle, pollPacket, 16, true);	

	return;
}

/**
 * Reset TX FIFO and initialize it with header bytes of DATA packet information.
 */
void HAL_RADIO_MASTER::initMessages(int packetLength)
{
	// The first 2 bytes contain lenght information (RAIL frame header).
	// The third byte indicates that it is a DATA packet.
	uint8_t header[3];

	this->packetLength = packetLength;	// This line is only used for Debug purposes.

	// Packet length information is stored MSB first.
	// It uses RAIL’s Frame Variable Length feature, using 12 bits to store length information along 2 bytes.
	// Visual guide of bit order:	x x x x 12 11 10 9 | 8 7 6 5  4 3 2 1
	// 12 bits allow for a maximum size of 4096 bytes, but the protocol limit is specified in the MAX_PACKET_LENGTH macro.
	// This specifications are stored in the *.isc file, generated by Simplicity Studio.
	header[0] = (packetLength + 1) >> 8;
	header[1] = (packetLength + 1) & 0xFF;
	
	// Note: this following byte is not part of the frame header, according to RAIL nomenclature.
	header[2] = 0x00;	// the "0x00" value in pollPacket[2] means the packet is a DATA packet.	

	RAIL_WriteTxFifo(railHandle, header, 3, true);
}

/****************SLAVE****************/

/**
 * State switching of Slave’s FSM.
 *
 * @return TRUE if a state switch occurs, otherwise returns FALSE.
 */
bool HAL_RADIO_SLAVE::switchState()
{
	switch(currentState) 
	{
		case SLAVE_STATE_LISTEN:
			if(pollReceived)
			{	
				if(sendData) 
					currentState = SLAVE_STATE_DATA;
				else
					currentState = SLAVE_STATE_LISTEN;
				pollReceived = 0;
				return true; 
			}
			break;	
		case SLAVE_STATE_DATA:
			if(packetSent) 
			{
				packetSent  = 0;
				currentState = SLAVE_STATE_LISTEN;
				return true; 
			}
			break;
	}
	return false;  
}

/**
 * State execution of Slave’s FSM.
 */
void HAL_RADIO_SLAVE::executeState()
{
	switch(currentState) 
	{
		case SLAVE_STATE_LISTEN:	listen(); 	break;
		case SLAVE_STATE_DATA:		data(); 	break;
	}
}

/**
 * Execution of state "LISTEN" of Slave’s FSM.
 */
void HAL_RADIO_SLAVE::listen()
{
	// Configure state transitions in RX state.
	RAIL_StateTransitions_t transitions = {RAIL_RF_STATE_RX, RAIL_RF_STATE_RX};
	RAIL_SetRxTransitions(railHandle, &transitions);

	RAIL_StartRx(railHandle, pollChannel, NULL);
}

/**
 * Execution of state "SEND DATA" of Slave’s FSM.
 */
void HAL_RADIO_SLAVE::data()
{	
	// Initialize TX FIFO with header information of the data packet.
	initMessages(tempBuffLength);	
	
	// Append data contained in tempBuff to the TX FIFO. 
	RAIL_WriteTxFifo(railHandle, tempBuff, tempBuffLength, false);
	
	// Schedule transmission of data at the instant "tx_time", which corresponds to the beginning of this Slave’s timeslot.
	// Note: While the master uses absolute timing, the slave uses relative timing (delay) to schedule it’s transmissions
	RAIL_ScheduleTxConfig_t scheduleTxConfig;
	scheduleTxConfig.when		= tx_delay;
	scheduleTxConfig.mode		= RAIL_TIME_DELAY;
	scheduleTxConfig.txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX;

	RAIL_Status_t status = RAIL_StartScheduledTx(railHandle, dataChannel, RAIL_TX_OPTIONS_DEFAULT, &scheduleTxConfig, NULL);
	
	// Failure to schedule data TX properly. 
	if (status != RAIL_STATUS_NO_ERROR)
	{
		// Ignore the failure, continue as if it was correctly scheduled and sent.
		packetSent = 1;	
	}
}

/**
 * Used to change protocol parameters during runtime.
 *
 * @return TRUE if valid parameters were introduced, otherwise returns FALSE.
 */
bool HAL_RADIO_SLAVE::config(RADIO_PARAMETER_TYPE type, int paramVal)
{
	switch (type)
	{
		case POLL_CHANNEL:
			if (paramVal < channelConfigs[0]->configs->channelNumberStart || paramVal > channelConfigs[0]->configs->channelNumberEnd)
				return false;
			else
				pollChannel = paramVal;
			break;
		case DATA_CHANNEL:
			if (paramVal < channelConfigs[0]->configs->channelNumberStart || paramVal > channelConfigs[0]->configs->channelNumberEnd)
				return false;
			else
				dataChannel = paramVal;
			break;
		case SLAVE_ID:
			if (paramVal < 0 || paramVal > MAX_NUMBER_SLAVES )
				return false;
			else
				id = paramVal;
			break;
		case PACKET_LENGTH:	// debug
			packetLength = paramVal;
			break;
		default: return false;
	}
	return true;
}

/**
 *  Called by RAIL event interrupt when packet reception occurs.
 */
void HAL_RADIO_SLAVE::storeReceivedPackets(uint16_t bytesAvailable) 
{
	static uint16_t rxReceived = 0;	// Number of bytes collected from RX FIFO.
	static uint16_t rxExpected = 0;	// Total number of bytes of the packet, according to header information.
	
	// If the previous packet was complete received with success, rxExpected will be 0.
	if(rxExpected == 0) 
	{
		rxReceived = RAIL_ReadRxFifo(railHandle, rxPtr, bytesAvailable);
		rxExpected = (rxPtr[0] << 8) + rxPtr[1] + PACKET_HEADER_LEN;

		// Packet was completely received.
		if(rxExpected == rxReceived) 
			rxExpected = 0;
	
		// It is a POLL packet, and this Slave was given a valid timeslot.
		// The Slave will transmit DATA after at least tx_delay.
		if (rxPtr[2] == 0xFF && rxPtr[3+id] != 0xFF)
		{
			tx_delay = ((RAIL_Time_t)rxPtr[3+id]*DATA_TIMESLOT_INTERVAL) + POLL_TIMESLOT_INTERVAL;
			pollReceived = 1;
		}
		
		// It is a DATA packet.
		else if (rxPtr[2] != 0xFF && rxExpected == 0) 
		{
			memcpy(lastDataPacket, rxPtr, rxReceived);
			lastDataPacketLength = rxReceived;
			dataReady = true;
		}

		// Collection of RX packet timestamps. Used for Debug purposes.
		rxDetails.timeReceived.totalPacketBytes = rxReceived;
		rxDetails.timeReceived.timePosition = RAIL_PACKET_TIME_AT_PACKET_END_USED_TOTAL;
		RAIL_GetRxPacketDetails(railHandle, RAIL_RX_PACKET_HANDLE_NEWEST, &rxDetails);
		if (++rxTimestampCounter < 100)
			rx_timestamp[rxTimestampCounter-1] = rxDetails.timeReceived;
	} 
	else  // the sender splits the messages. concatenation is needed. 
	{ 
		uint16_t _rxReceived = rxReceived;
		uint16_t _rxExpected = rxExpected;
		rxReceived += RAIL_ReadRxFifo(railHandle, rxPtr + _rxReceived, _rxExpected - _rxReceived);
	}
}

/**
 * Reset TX FIFO and initialize it with header bytes of DATA packet information.
 */
void HAL_RADIO_SLAVE::initMessages(int packetLength)
{
	// The first 2 bytes contain lenght information (RAIL frame header).
	// The third byte indicates that it is a DATA packet.
	uint8_t header[3];

	this->packetLength = packetLength; // This line is only used for Debug purposes.

	// Packet length information is stored MSB first.
	// It uses RAIL’s Frame Variable Length feature, using 12 bits to store length information along 2 bytes.
	// Visual guide of bit order:	x x x x 12 11 10 9 | 8 7 6 5  4 3 2 1
	// 12 bits allow for a maximum size of 4096 bytes, but the protocol limit is specified in the MAX_PACKET_LENGTH macro.
	// This specifications are stored in the *.isc file, generated by Simplicity Studio.
	header[0] = (packetLength + 1) >> 8; 
	header[1] = (packetLength + 1) & 0xFF;
	
	// Note: this following byte is not part of the frame header, according to RAIL nomenclature.
	header[2] = 0x00;	// the "0x00" value in this position means the packet is a DATA packet.	
	
	RAIL_WriteTxFifo(railHandle, header, 3, true);
}

} // end namespace RODOS
	
