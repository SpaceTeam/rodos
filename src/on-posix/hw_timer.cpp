

/**
* @file hw_timer.cc
* @date 2008/04/23 16:36
* @author Lutz Dittrich
*
*
* @brief fixed interval timer
*
* class for fixed-interval timer  for Linux as guest os
* uses the virtual timer (counts only when process is executed)
*/

// #include <stdio.h>
// #include <stdlib.h>

#include <signal.h>

#include "hw_specific.h"
#include "rodos.h"
#include <sys/time.h>


/**
* The instruction pointer during the last interruption.
*/
volatile long __interruptedInstructionPointer__ = 0;

extern "C" void exit(int c);

namespace RODOS {

/**
* Size of stack required by signal handler.
*/
#define SIGNAL_HANDLER_STACK_SIZE SIGSTKSZ

/** the timer interval */
long long Timer::microsecondsInterval = 0;


/**
* the signal handler for SIGVTALRM (timer signal)
*/
void timerSignalHandler(int ignore);
void timerSignalHandler(int ignore) {

    // time events to call?
    int64_t timeNow = NOW();       // comment this out to improve performance, but: no time events any more
    TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more
}

/**
* initialize the timer and signal handler 
*/
void Timer::init() {
    //Signal handlers are called synchronously from the main thread, see scheduler.cpp
    //signal(SIGALRM, timerSignalHandler);
}

/**
* start timer 
*/
void Timer::start() {
    itimerval params;
    int       retval;
    params.it_interval.tv_sec  = microsecondsInterval / 1000000;
    params.it_interval.tv_usec = microsecondsInterval % 1000000;
    params.it_value.tv_sec     = microsecondsInterval / 1000000;
    params.it_value.tv_usec    = microsecondsInterval % 1000000;
    retval                     = setitimer(ITIMER_REAL, &params, 0);
    if(retval != 0) {
        xprintf("error during call to setitimer\n");
        exit(1);
    }
}

/**
* stop timer 
*/
void Timer::stop() {
    struct itimerval params;
    int              retval;
    params.it_interval.tv_sec  = 0;
    params.it_interval.tv_usec = 0;
    params.it_value.tv_sec     = 0;
    params.it_value.tv_usec    = 0;
    retval                     = setitimer(ITIMER_REAL, &params, 0);
    if(retval != 0) {
        xprintf("error during call to setitimer\n");
        exit(1);
    }
}

/**
* set timer interval 
*/
void Timer::setInterval(const long long microsecondsInterval) {
    Timer::microsecondsInterval = microsecondsInterval;
}

} // namespace RODOS
