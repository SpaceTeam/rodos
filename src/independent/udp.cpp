



#include "rodos.h"
/**
 * @file udp.h
 * @date 2008/05/20 6:20
 * @author Sergio Montenegro
 *
 *
 * UDP interface
 *
 */


namespace RODOS {

UDPIn::UDPIn(const long portNr) : udpRx(portNr) 			      { }
UDPIn::UDPIn(const long portNr, Topic<GenericMsgRef>* associatedTopic) : udpRx(portNr)  {udpRx.setAsync(associatedTopic); }

void UDPIn::setAsync(Topic<GenericMsgRef>* associatedTopic)             {udpRx.setAsync(associatedTopic); }

void UDPIn::reopen(const long portNr) 					{ udpRx.reopen(portNr); }

long UDPIn::get(void* userData, int maxLen) 				{ return udpRx.get(userData, maxLen); }
long UDPIn::get(void* userData, int maxLen, unsigned long *ipaddr) 	{ return udpRx.get(userData, maxLen,ipaddr); }
bool UDPIn::readyToGet() 						{ return udpRx.readyToGet(); }

bool UDPIn::isConnected() 						{ return udpRx.isInitialised(); }

#ifdef MULTICAST
// to use this method please define the preprocessor symbol (-D) MULTICAST in your compiler
void UDPIn::joinMulticastGroup(unsigned long ipaddr) { udpRx.joinMulticastGroup(ipaddr);}
#endif

/****************/

UDPOut::UDPOut(const long portNr, const char* hostname) 		: udpTx(portNr, hostname) 	{ }
//UDPOut::UDPOut(const long _portNr) 					: udpTx(_portNr) 	  	{ }
UDPOut::UDPOut(const long _portNr, unsigned long _ipAddr) 		: udpTx(_portNr, _ipAddr)	{ }
UDPOut::UDPOut(const long _portNr, int ip0, int ip1, int ip2, int ip3)	: udpTx(_portNr, ip0, ip1, ip2, ip3) { }

bool UDPOut::send(const void* userData, const int maxLen) 	        	 { return udpTx.send(userData, maxLen); }
bool UDPOut::sendTo(const void* userData, const int maxLen, unsigned long ipAddr) { return udpTx.sendTo(userData, maxLen,ipAddr); }
bool UDPOut::isConnected() 						{ return udpTx.isInitialised(); }


UDPInOut::UDPInOut(int broadcastPort):
    udpIn(broadcastPort),
    udpOut(broadcastPort) {
    isBroadcastLink=true;
    if(broadcastPort >=0) {
        ERROR("Broadcast UDPInOut defined but port number ist positive");
    }
}

UDPInOut::UDPInOut(int port ,const char* hostname):
    udpIn(port),
    udpOut(port,hostname) {
    isBroadcastLink=false;
    if(port <=0) {
        ERROR("Non-Broadcast UDPInOut defined but port number ist negative");
    }
}

UDPInOut::UDPInOut(int localInputPort, int remoteOutputPort,const  char* hostname):
    udpIn(localInputPort),
    udpOut(remoteOutputPort,hostname) {
    isBroadcastLink=false;
    if(localInputPort <=0 || remoteOutputPort <=0) {
        ERROR("Non-Broadcast UDPInOut defined but port number ist negative");
    }
}


}

