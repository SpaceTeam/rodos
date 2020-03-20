/**
* @file thread.cc
* @date 2008/04/22 16:50
* @author Lutz Dittrich, Sergio Montenegro
*
*
* A Thread is a schedulable object with own context and stack
*
* @brief %Thread handling
*/

#include "rodos-debug.h"
#include "thread.h"
#include "timepoints.h"


namespace RODOS {

/********************************************/

void StacklessThread::setPeriodicBeat(const int64_t begin,
                                      const int64_t period) {
    nextBeat = begin;
    this->period = period;
}

void StacklessThread::suspendUntilNextBeat() {
    while (period == 0) {
        RODOS_ERROR("Thead with Period 0 waiting for next beat");
        suspendCallerUntil(); // WRONG! Period shall not be 0 for beats
    }

    suspendCallerUntil(nextBeat);
    int64_t timeNow = NOW();
    nextBeat = TimeModel::computeNextBeat(nextBeat, period, timeNow);
}


/********************************************************/


long StacklessThread::setPrioCurrentRunner(long newPrio) {
    StacklessThread* runner = getCurrentThread();
    if(runner==0) {
        errorLog.addRaw("null pointer setPrioCurrentRunner");
        return 0;
    }
    long previusPriority = runner->getPriority();
    runner->setPriority(newPrio);
    return previusPriority;
}


volatile int32_t taskRunning = 0;

}

