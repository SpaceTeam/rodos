
/**
 * @file params.h
 * @date 2012/08/20 16:13
 * @author Michael Ruffer
 *
 * Copyright 2012 University Wuerzburg
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define OSVERSION "STM32F4 Cortex-M4 V1.0"

/*************** System Configuration *********/

/** system clocks
 * - APB1_CLK is fixed to CPU_CLK/4 (TIM2/3/4/5/6/7/12/13/14, USART2/3, UART4/5, SPI2/3, I2C1/2/3, CAN1/2, DAC1/2, WWDG)
 * - APB2_CLK is fixed to CPU_CLK/2 (TIM1/8/9/10/11, USART1/6, SPI1, ADC1/2/3, MMC)
 * - for more information see system_stm32f4xx.c
 */
// MINI-M4, STM32F4Discovery
#define CPU_CLK                         168   // 24,32,42,48,56,84,96,120,144,168 MHz


/** uart for printf **/
#define UART_DEBUG                      UART_IDX2, GPIO_053, GPIO_054 // PD5 and PD6
//#define UART_DEBUG                      UART_IDX3, GPIO_056, GPIO_057 // PD8 and PD9
//#define UART_DEBUG                      UART_IDX4, GPIO_000, GPIO_001 // PA0 and PA1
//#define UART_DEBUG                      UART_IDX5

#define XMALLOC_SIZE		40*1024
#define DEFAULT_STACKSIZE	2000

/** stack size (in bytes) for scheduler
 * - ISRs and Scheduler are using the main stack (MSP)
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 * - So we must provide enough stack in the linker script (see "_Min_Stack_Size" in stm32_flash.ld)!
 */
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

// Declare global variables with this to put them into the 64k-CoreCoupled Memory Block
// No DMA is possible. No initialization is possible,all gets set to zero.
#define PUT_INTO_CCRAM __attribute__ ((section (".bss_ccram")))


