#include <string.h>
#include "rodos.h"
#include "hw_udp.h"

namespace RODOS {

UDPReceiver::UDPReceiver(const int port) {}
UDPReceiver::~UDPReceiver() {}
void UDPReceiver::reopen(const int port) {}

/*
bool UDPReceiver::isConnected() {
	if(pcbUdp == NULL)
		return false;
	else
		return true;
}
*/

bool UDPReceiver::readyToGet() { return false; }

void UDPReceiver::setAsync(Topic<GenericMsgRef>* associatedTopic) {}

long UDPReceiver::get(void* userData, const unsigned int maxLen) { return 0; }
long UDPReceiver::get(void* userData, int maxLen, unsigned long* ipaddr) { return 0; }

UDPTransmitter::UDPTransmitter(const int port, const char* host) {}
UDPTransmitter::UDPTransmitter(const long _portNr, unsigned long _ipAddr) {}
UDPTransmitter::UDPTransmitter(const long _portNr, int ip0, int ip1, int ip2, int ip3) {}
UDPTransmitter::~UDPTransmitter() {}

void UDPTransmitter::openConnection(const int port, const char* host) {}

bool UDPTransmitter::send(const void* msg, const unsigned int len) { return false; }
bool UDPTransmitter::sendTo(const void* userData, const int maxLen, unsigned long _ipAddr) { return false; }

} // namespace RODOS
