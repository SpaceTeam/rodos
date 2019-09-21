
#pragma once

#include "hw_tcp.h"

/**
 * @file tcp.h
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 * TCP interface
 */

namespace RODOS {

class TCPServer {
protected:
  HW_TCPServer tcp;

public:
  bool listen(const long portNr);
  bool acceptNewConnection();
  int sendData(void* buf, int len);   // not just send, due to name collision with lwip
  int getData(void* buf, int maxLen); // not just get due to name collision
  int getErrorCode();
};

class TCPClient {
protected:
  HW_TCPClient tcp;

public:
  bool reopen(const long portNr, const char* hostname = "localhost");
  int sendData(void* buf, int len);   // not just send due to name collision with lwip
  int getData(void* buf, int maxLen); // not just get due to name collision
  int getErrorCode();
};

}  // namespace
