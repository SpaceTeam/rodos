

/**
* @file hw_specific.cc
* @date 2010/04/23 7:33
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief all hardware specific stuff I have no better place for ...
*
*/

#include "hw_specific.h"
#include "rodos.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

extern "C" void abort();

namespace RODOS {

/**
* initialize hardware specific stuff
*/

extern void sigtermHandler(int sig);
extern long myNodeNr;

void hwInit() {
    taskRunning = 0;

    /* External interrupts simulator */
    signal(SIGTERM, sigtermHandler);

    /** best effort to determine the node noumber (not perfect or even ok!).
     * This number may be changed by the gateway link interface
     * according to other chriteria **/
    // myNodeNr = (getpid() % (MAX_NUMBER_OF_NODES -1) ) + 1; ORIGINAL
    myNodeNr = getpid(); // a default node number, may be replaced by the link interface (gateway)
}

/**
*  abort execution
*/
void hwResetAndReboot() {
    xprintf("hw_resetAndReboot() -> exit");
    abort();
}

// cpu core and periferis off until extern interrupt or time "until"
void deepSleepUntil(int64_t until) {
    int64_t  deltaT = until - NOW();
    timespec deltaTPosix;
    timespec remainingTime;
    deltaTPosix.tv_sec  = deltaT / SECONDS;
    deltaTPosix.tv_nsec = deltaT % SECONDS;

    hwDisableInterrupts();
    nanosleep(&deltaTPosix, &remainingTime);
    hwEnableInterrupts();
}


static long long timeOfLastTriggerWatchdog = 0;
void hwInitWatchdog(long intervalMilliseconds) { }
void hwTriggerWatchdog() { timeOfLastTriggerWatchdog = NOW(); }

/**
* create context on stack and return a pointer to it
*/
long* hwInitContext(long* stack, void* object) {
    return (long*)0;
}


/******************************************/
void sp_partition_yield() {}

void FFLUSH() { fflush(stdout); }

} // namespace RODOS
