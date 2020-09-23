/*
 * Copyright (c) 2020 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Jorge Oliveira, Marcelo Alves
 */

#pragma once


#include "rail-headers.h"
#include "platform-parameter.h"

#include <stdint.h>

namespace RODOS {

extern "C" {
RAIL_Status_t RAILCb_SetupRxFifo(RAIL_Handle_t railHandle);
void          RAILCb_AssertFailed(RAIL_Handle_t railHandle, RAIL_AssertErrorCodes_t errorCode);
void          RAIL_callback(RAIL_Handle_t railHandle, RAIL_Events_t events);
}

// protocol parameters that can be changed in runtime. 
enum RADIO_PARAMETER_TYPE {
    POLL_CHANNEL,
    DATA_CHANNEL,
    NUMBER_OF_SLAVES,
    SLAVE_ID,
    PACKET_LENGTH // Only for Debug purposes.
};

enum RADIO_STATUS_TYPE {
    RADIO_STATUS_OK,
    RADIO_STATUS_ERROR
};

/**
 * This class contains variables and methods common to both Master and Slave devices of the protocol.
 */
class HAL_RADIO : public GenericIOInterface {

  private:
    friend RAIL_Status_t RAILCb_SetupRxFifo(RAIL_Handle_t railHandle);
    friend void          RAILCb_AssertFailed(RAIL_Handle_t railHandle, RAIL_AssertErrorCodes_t errorCode);
    friend void          RAIL_callback(RAIL_Handle_t railHandle, RAIL_Events_t events);

    // Buffers used as RAIL's internal FIFOs for transmitting and receiving packets.
    uint8_t txFifo[TX_FIFO_SIZE];
    uint8_t rxFifo[RX_FIFO_SIZE];

  protected:
    RAIL_Handle_t railHandle = NULL;

    uint16_t pollChannel = 0;  // different radio chanels to poll and data transmission
    uint16_t dataChannel = 1;  // slaves hear pollChanel, master hears dataChannel

    uint8_t rxPtr[MAX_NETWORK_MESSAGE_LENGTH];

    // RAIL Callback flags
    bool packetSent = false;

    uint8_t currentState = 0;

    // Auxiliary variables used with the write and read functions
    bool sendData  = false;
    bool dataReady = false;
    // Temporary buffer to store the data to be sent
    uint8_t tempBuff[MAX_NETWORK_MESSAGE_LENGTH];
    uint16_t     tempBuffLength = 0;

    virtual void storeReceivedPackets(uint16_t bytesAvailable) = 0;
    virtual void initMessages(int packetLength)                = 0;

    uint16_t rxReceived = 0; // bytes Received

    // Last received data packet
    uint8_t  lastDataPacket[MAX_NETWORK_MESSAGE_LENGTH];
    uint16_t lastDataPacketLength;

    // Only for Debug purposes.
    int packetLength = 16; // debuging purposes -> used on the Callback

  public:
    int8_t lastPacketRssi = 0;
    HAL_RADIO();
    ~HAL_RADIO() {};

    // Device and protocol initialization.
    RADIO_STATUS_TYPE init();

    // Used to change protocol parameters during runtime.
    // @see hal_radio_mstrslv.cpp
    virtual bool config(RADIO_PARAMETER_TYPE type, int paramVal) = 0;

    // Makes internal copy of sendBuff for later transmission via radio.
    RADIO_STATUS_TYPE write(const char* sendBuff, int len);

    // Stores latest available data from radio in recBuff.
    int read(char* recBuf);

    bool isWriteFinished();

    // Only for Debug purposes.
    volatile uint16_t      txOffset;
    volatile uint16_t      txLength;
    RAIL_TxPacketDetails_t txDetails;
    RAIL_RxPacketDetails_t rxDetails;
    RAIL_PacketTimeStamp_t tx_timestamp[100];
    RAIL_PacketTimeStamp_t rx_timestamp[100];
    volatile int           txTimestampCounter = 0;
    volatile int           rxTimestampCounter = 0;

    volatile int pollPackets = 0;
    volatile int dataPackets = 0;

};

} // namespace RODOS
