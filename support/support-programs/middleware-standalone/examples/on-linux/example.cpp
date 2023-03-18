/*
 * main.cpp
 *
 *  Created on: 10.07.2013
 *      Author: Erik
 */


#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include "rodos-middleware.h"


UDPInOut udp(-50000);
LinkinterfaceUDP linkif(&udp);
static Gateway gw(&linkif,true);

#define ON_LINUX

//#define ON_LINUX //if not defined: stm32f4


#ifdef ON_LINUX

static HAL_UART uart(UART_IDX4);

int64_t NOW(){
	struct timeval t;
	gettimeofday(&t,0);
	return (t.tv_sec*1000000LL+t.tv_usec)*1000;
}

#else
#include "stm32f4xx.h"

static HAL_UART uart(UART_IDX3);

HAL_UART uart_stdout(UART_IDX2);

int64_t NOW(){
	static int32_t faketime=0;
	return faketime++;
}

#endif



/*
static HAL_CAN can(CAN_IDX1);
static LinkinterfaceCAN linkif(&can);

//static LinkinterfaceUART linkif(&uart);

static Gateway gw(&linkif);

*/

struct Position {
	char someData[3];
	long cntr;
	float x, y, z;
} __attribute__((packed));

int32_t timer=0;



class MessageHandler : public Putter {


	virtual bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) override {

		Position* p;

                printf("\n Got Data");

		if(topicId==1003){



			p=(Position*)msg;

			if(p->cntr> timer){
				timer=p->cntr;

			}


			printf("Got clock len=%d data=%d timer=%d\n",len,(int)p->cntr,(int)timer);

		}else{
			printf("Got other len=%d topicID=%ld\n",len,topicId);
		}

		return true;
	}




}msgHandler;




int main(){

	Position outMsg;
#ifndef ON_LINUX
SystemCoreClockUpdate();
uart_stdout.init(115200);
#endif

	//can.init(100000);
		uart.init();

	printf("Hello World!\n");

	gw.init(3);
	gw.setPutter(&msgHandler);



	while(1){


                gw.pollMessages();

		outMsg.cntr=timer++;

		//printf("sending...\n");

		gw.sendNetworkMessage((char*)&outMsg,sizeof(outMsg),1002,NOW());

		printf("Sended\n");
		usleep(500000);



	}












return 0;
}


