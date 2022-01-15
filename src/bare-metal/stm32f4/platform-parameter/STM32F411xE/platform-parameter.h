/**
 * @file params.h
 * @date 2012/08/20 16:13
 * @author Michael Ruffer
 *
 * Copyright 2012 University Wuerzburg
 *
 * @brief configuration parameters ...
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define OSVERSION "STM32F4 Cortex-M4 V1.0"

/*************** System Configuration *********/

/** system clocks
 * - APB1_CLK is fixed to CPU_CLK/2 (TIM2/3/4, USART2, SPI2/3, I2C1/2/3, WWDG)
 * - APB2_CLK is fixed to CPU_CLK/1 (TIM1/9/10/11, USART1/6, SPI1/4/5, ADC1/, MMC)
 * - for more information see system_stm32f4xx.c
 */
#define CPU_CLK                         96   // 24,32,42,48,56,84,96,100 MHz

/** uart for printf **/													//	Tx      Rx
//#define UART_DEBUG                      UART_IDX1, GPIO_009, GPIO_010 // PA9 and PA10
#define UART_DEBUG                      UART_IDX2, GPIO_053, GPIO_054 // PD5 and PD6
//#define UART_DEBUG                      UART_IDX6, GPIO_038, GPIO_039 // PC6 and PC7


#undef  XMALLOC_SIZE
#define XMALLOC_SIZE		40*1024
#undef  DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE	2000

/** stack size (in bytes) for scheduler
 * - ISRs and Scheduler are using the main stack (MSP)
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 * - So we must provide enough stack in the linker script (see "_Min_Stack_Size" in stm32_flash.ld)!
 */
#undef  SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE 		8

#undef  UDP_INCOMMIG_BUF_LEN
#define UDP_INCOMMIG_BUF_LEN        4 //number of "NetworkMessage (1300 Bytes each)" in FIFO
