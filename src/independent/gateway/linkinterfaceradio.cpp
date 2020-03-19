#include "gateway/linkinterfaceradio.h"
#include "hw_specific.h"

#include "gateway/gateway.h"


namespace RODOS {

/**
 * @file linkinterfaceudp-broadcast.cc
 * @date 2012/09/01, 2009/05/18, 2012
 * @author Sergio Montenegro, uni Würzburg
 *
 * Link Interface to broadcast networks simulated using RADIO messages.
 * incomming messages will be distributed asyncrhonously using the middleware.
 *
 */


/****************************************************/

LinkinterfaceRADIO::LinkinterfaceRADIO(HAL_RADIO_MASTER* master, HAL_RADIO_SLAVE* slave, bool isMaster, long identifier) 
	: Linkinterface(identifier)
{ 
	this->master = master;
	this->slave = slave;

	if (isMaster) 
		radio = master;
	else
		radio = slave;
}


void LinkinterfaceRADIO::init() {
	Linkinterface::init();

	threadToResume= Thread::getCurrentThread();
}

/************* Connection to the network, network and protocol dependent ***/

bool LinkinterfaceRADIO::sendNetworkMsg(NetworkMessage &outMsg)
{
	radio->write((char*)&outMsg, outMsg.numberOfBytesToSend());
	return true;
}

bool LinkinterfaceRADIO::getNetworkMsg(NetworkMessage &inMsg, int32_t &numberOfReceivedBytes)
{
	numberOfReceivedBytes = radio->read((char*)&inMsg);
	
	if (numberOfReceivedBytes > 0) 
		return true;
	else
		return false;
}

void LinkinterfaceRADIO::switchToMaster()
{
	radio = master;
	radio->init();
}

void LinkinterfaceRADIO::switchToSlave()
{
	radio = slave;
	radio->init();
}

void LinkinterfaceRADIO::onWriteFinished()
{
	return;
}

void LinkinterfaceRADIO::onDataReady()
{
	return;
}

void LinkinterfaceRADIO::suspendUntilDataReady(int64_t reactivationTime){
	//Not yet race condition safe
	Thread::suspendCallerUntil(NOW()+100*MILLISECONDS);
}

}
