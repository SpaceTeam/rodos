/*
 * Copyright (c) 2020 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Jorge Oliveira, Marcelo Alves
 */

#pragma once

#include "hal/hal_radio.h"

namespace RODOS {

// This class implements variables and methods specific to Master devices.
class HAL_RADIO_MASTER : public HAL_RADIO {

  private:
    int         numberOfSlaves = INIT_NUMBER_SLAVES;
    int         slave_rx_counter[MAX_NUMBER_SLAVES];

    void poll();
    void listen();
    void data();
    void storeReceivedPackets(uint16_t bytesAvailable);
    void initMessages(int packetLength);
    void initPoll();

  public:
    bool switchState();
    void executeState();
    bool config(RADIO_PARAMETER_TYPE type, int paramVal);
};

// This class implements variables and methods specific to Master devices.
class HAL_RADIO_SLAVE : public HAL_RADIO {

  private:
    int                  id = 0;
    volatile RAIL_Time_t tx_delay;

    // Flag used in storeReceivedPackets
    bool pollReceived = false;

    void listen();
    void data();
    void storeReceivedPackets(uint16_t bytesAvailable);
    void initMessages(int packetLength);

  public:
    bool switchState();
    void executeState();
    bool config(RADIO_PARAMETER_TYPE type, int paramVal);
};

} // namespace RODOS
