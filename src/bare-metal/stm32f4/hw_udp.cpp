#include <string.h>
#include "rodos.h"
#include "hw_udp.h"


namespace RODOS {


UDPReceiver::UDPReceiver([[gnu::unused]] const int port) { }
UDPReceiver::~UDPReceiver() { }
void UDPReceiver::reopen([[gnu::unused]] const int port) { }

/*
bool UDPReceiver::isConnected() {
	if(pcbUdp == NULL)
		return false;
	else
		return true;
}
*/

bool UDPReceiver::readyToGet() { return false; }

void UDPReceiver::setAsync([[gnu::unused]] Topic<GenericMsgRef>* associatedTopic){ }

long UDPReceiver::get([[gnu::unused]] void* userData, [[gnu::unused]] const unsigned int maxLen) { return 0; }
long UDPReceiver::get([[gnu::unused]] void* userData, [[gnu::unused]] int maxLen, [[gnu::unused]] unsigned long *ipaddr) { return 0; }

UDPTransmitter::UDPTransmitter([[gnu::unused]] const int port, [[gnu::unused]] const char *host) { }
UDPTransmitter::UDPTransmitter([[gnu::unused]] const long _portNr, [[gnu::unused]] unsigned long _ipAddr) { }
UDPTransmitter::UDPTransmitter([[gnu::unused]] const long _portNr, [[gnu::unused]] int ip0, [[gnu::unused]] int ip1, [[gnu::unused]] int ip2, [[gnu::unused]] int ip3) { }
UDPTransmitter::~UDPTransmitter() { }

void UDPTransmitter::openConnection([[gnu::unused]] const int port, [[gnu::unused]] const char *host) { }

bool UDPTransmitter::send([[gnu::unused]] const void*msg, [[gnu::unused]] const unsigned int len) { return false; }
bool UDPTransmitter::sendTo([[gnu::unused]] const void* userData, [[gnu::unused]] const int maxLen, [[gnu::unused]] unsigned long _ipAddr) { return false; }

}

