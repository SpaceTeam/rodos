#include "rodos.h"

HAL_UART uart1(UART_IDX2);
HAL_UART uart2(UART_IDX1);
HAL_UART uart3(UART_IDX4);

LinkinterfaceUART linkinterfaceUART1(&uart1, 115200);
LinkinterfaceUART linkinterfaceUART2(&uart2, 115200);
LinkinterfaceUART linkinterfaceUART3(&uart3, 115200);

Gateway gateway1(&linkinterfaceUART1, true);
Gateway gateway2(&linkinterfaceUART2, true);
Gateway gateway3(&linkinterfaceUART3, true);

Router router(false, &gateway1, &gateway2, &gateway3);

