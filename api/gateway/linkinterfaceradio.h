/*
 * linkinterfaceradio.h
 *
 *  Created on: 17.02.2020
 *      Author: Jorge Oliveira & Marcelo Alves
 */

#pragma once

#include "gateway/linkinterface.h"
#include "rodos.h"
#include "hal/hal_radio_mstrslv.h"

namespace RODOS {

class LinkinterfaceRADIO : public Linkinterface, IOEventReceiver {
private:
	HAL_RADIO* radio;
	HAL_RADIO_MASTER* master; 
	HAL_RADIO_SLAVE* slave;

public:

	LinkinterfaceRADIO(HAL_RADIO_MASTER* master, HAL_RADIO_SLAVE* slave, bool isMaster, long identifier);

    virtual ~LinkinterfaceRADIO() { }
    /**
     * Link interface to the gateway, look up a free RADIO input port and
     * assign a corresponding node number used as ID in the network.
     */
    void init();

    /**
     * Retrieve a message in network potocol format from an associated RADIO port.
     * @param outMsg pointer to message to send wrapped in network structure
     * @return true - if sending was successful, false - otherwise
     */
    bool sendNetworkMsg(NetworkMessage &outMsg);
    bool getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes);

	void switchToMaster();
    void switchToSlave();

    void onWriteFinished();
    void onDataReady();

    virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);
};

}