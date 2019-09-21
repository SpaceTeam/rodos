
/**
 * @file params.h
 * @date 2019/08/20
 * @author Sergio Montenegro
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define OSVERSION "Template"

/*************** System Configuration *********/

#define XMALLOC_SIZE		40*1024
#define DEFAULT_STACKSIZE	2000
#define SCHEDULER_STACKSIZE 		8
#define PARAM_TIMER_INTERVAL		10000
#define TIME_SLICE_FOR_SAME_PRIORITY	(100*MILLISECONDS)
#define DEFAULT_THREAD_PRIORITY		100
#define MAX_THREAD_PRIORITY		1000
#define NETWORKREADER_PRIORITY		(MAX_THREAD_PRIORITY + 2)
#define FLOORING_PRIORITY               10
#define CEILING_PRIORITY		(NETWORKREADER_PRIORITY + 1)
#define MAX_NUMBER_OF_NODES             10 //< for gateway if it has to forward msgs
#define MAX_NETWORK_MESSAGE_LENGTH	1300
#define MAX_SUBSCRIBERS			60

