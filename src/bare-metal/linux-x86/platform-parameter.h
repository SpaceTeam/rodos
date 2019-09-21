
/**
* @file platform-parameter.h
* @date 2008/04/24 16:13
* @author Lutz Dittrich, Sergio Montenegro
*
*
* @brief configuration parameters for the current platform
*
*/

#pragma once

// #include "hw_datatypes.h"

// ___________________  only defines, not in namespace


#define OSVERSION "Linux-x86"

/*************** System Configuration *********/

#define XMALLOC_SIZE        2000000 //< Total memory for allocation (xmalloc) eg for all stacks
#define DEFAULT_STACKSIZE     32000 //< in bytes
#define SCHEDULER_STACKSIZE   DEFAULT_STACKSIZE  //< for the scheduler

#define PARAM_TIMER_INTERVAL         100000 //< in microseconds
#define TIME_SLICE_FOR_SAME_PRIORITY (100*MILLISECONDS) //< for switch threads with same priority

#define DEFAULT_THREAD_PRIORITY  100
#define MAX_THREAD_PRIORITY     1000

#define NETWORKREADER_PRIORITY (MAX_THREAD_PRIORITY + 2) //< for priority ceiling
#define CEILING_PRIORITY       (NETWORKREADER_PRIORITY + 1)
#define FLOORING_PRIORITY      10

#define MAX_NUMBER_OF_NODES          10 //< for gateway if it has to forward msgs
#define MAX_NETWORK_MESSAGE_LENGTH 1300
#define MAX_SUBSCRIBERS              60 //< per node, for the gateway topic reporter

//#define UART_GATEWAY                //< activates the Interrupt fo incomming networkmessages for the UART Gateway
//#define ENABLE_LINUX_CAN_INTERRUPT  //< Uncomment to enable Linux CAN Interrupt (this may hang when using UDP!!)
