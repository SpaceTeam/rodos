#include "hal/tcp.h"

namespace RODOS {

bool TCPServer::listen(const long portNr)     { return tcp.listen(portNr); }
bool TCPServer::acceptNewConnection()         { return tcp.acceptNewConnection(); }
int TCPServer::sendData(void *buf, size_t len)   { return tcp.sendData(buf, len); }
int TCPServer::getData(void *buf, size_t maxLen) { return tcp.getData(buf, maxLen); }
int TCPServer::getErrorCode()                 { return tcp.getErrorCode(); }

bool TCPClient::reopen(const long portNr, const char *hostname) { return tcp.reopen(portNr, hostname); }
int TCPClient::sendData(void *buf, size_t len)                     { return tcp.sendData(buf, len); }
int TCPClient::getData(void *buf, size_t maxLen)                   { return tcp.getData(buf, maxLen); }
int TCPClient::getErrorCode()                                   { return tcp.getErrorCode(); }

}
