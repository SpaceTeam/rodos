
/**
 * @file platform-parameter.h
 * @date 2019/08/20
 * @author Marcelo Alves, Jorge Oliveira
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define		OSVERSION "EFR32FG1P Cortex-M4 V1.0"

/*************** System Configuration *********/

/** Memory for allocations, used for all stacks */
#undef  	XMALLOC_SIZE
#define 	XMALLOC_SIZE					8*1024	

/** Default stack size (in bytes) for threads */
#undef 	 	DEFAULT_STACKSIZE
#define 	DEFAULT_STACKSIZE				2000 

/** Default stack size (in bytes) for the scheduler */
#undef  	SCHEDULER_STACKSIZE
#define 	SCHEDULER_STACKSIZE				8	

/** Time interval between timer interrupts in microseconds */
#undef 		PARAM_TIMER_INTERVAL
#define 	PARAM_TIMER_INTERVAL			10000

/** Time interval between context switches under priority conflict */
#undef		TIME_SLICE_FOR_SAME_PRIORITY
#define		TIME_SLICE_FOR_SAME_PRIORITY	2000

/** Default priority for newly created threads */
#undef 		DEFAULT_THREAD_PRIORITY		
#define		DEFAULT_THREAD_PRIORITY			100

/** User threads shall not have a priority higher than this */
#undef 		MAX_THREAD_PRIORITY	
#define		MAX_THREAD_PRIORITY				1000

/** High priority levels for priority ceiling */
#undef		NETWORKREADER_PRIORITY
#define		NETWORKREADER_PRIORITY			(MAX_THREAD_PRIORITY + 1)

#undef		CEILING_PRIORITY	
#define		CEILING_PRIORITY				(NETWORKREADER_PRIORITY + 1)

/** Using a network, the maximal number of nodes attached */ 
#undef		MAX_NUMBER_OF_NODES	
#define 	MAX_NUMBER_OF_NODES				10

/** If using network it may have a limit for packets, eg. UDP */
#undef		MAX_NETWORK_MESSAGE_LENGTH	
#define		MAX_NETWORK_MESSAGE_LENGTH		1536


