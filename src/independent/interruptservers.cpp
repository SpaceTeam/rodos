

/**
* @file interruptservers.cc
* @date 2011/12/22 17:08
* @author Sergio Montenegro
*
*/
#include "rodos.h"


namespace RODOS {

/*********************************************/
void sigtermHandler(int sig);
void uartHandler(int sig);
void timerHandler(int sig);


void sigtermHandler(int sig)   { interruptSigterm.publishFromInterrupt(0); }
void uartHandler(int sig)      { interruptUart.publishFromInterrupt(0); }
void timerHandler(int sig)     { interruptTimer.publishFromInterrupt(0); }


}

