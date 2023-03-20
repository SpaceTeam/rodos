/*
 * main.cpp
 *
 *  Created on: 10.07.2013
 *      Author: Erik
 */

#include <stdio.h>
#include <stdlib.h>

#include "rodos-middleware.h"

#include <sys/time.h>
#include <unistd.h>

// HAL_UART uart_stdout(UART_IDX2);

UDPInOut udp(-50000);
LinkinterfaceUDP linkif(&udp);
static Gateway gw(&linkif);

struct Position {
  char someData[3];
  long cntr;
  float x, y, z;
} __attribute__((packed));

int32_t timer = 0;

int64_t NOW() {
  struct timeval t;
  gettimeofday(&t, 0);
  return (t.tv_sec * 1000000LL + t.tv_usec) * 1000;
}

class MessageHandler : public Putter {

public:
  virtual bool putGeneric(const long topicId, const unsigned int len, const void *msg, const NetMsgInfo &netMsgInfo) {

    if (topicId == 1003) {
      Position *p;
      p = (Position *)msg;
      if (p->cntr > timer) {
        timer = p->cntr;
      }
      printf("Got clock len=%d data=%d timer=%ld\n", len, (int)p->cntr, (long)timer);

    } else {
      printf("Got other topic len=%d topicID=%d\n", len, (int)topicId);  
    }
    return true;
  }

} msgHandler;

int main() {

  Position outMsg;

  printf("Hello World!\n");

  gw.init(3); // 3 is my simulated node Id 
  gw.setPutter(&msgHandler);

  while (1) {
    gw.pollMessages();
    outMsg.cntr = timer++;
    gw.sendNetworkMessage((char *)&outMsg, sizeof(outMsg), 1002, NOW()); // 1002 ist topic counter6 in rodos example
    printf("Sended %d\n", (int)timer);
    usleep(500000);
  }
  return 0;
}
