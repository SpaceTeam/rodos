

/**
* @file scheduler.cc
* @date 2008/04/24 18:10
* @author Lutz Dittrich
*
* @brief priority based scheduler
*
*/
#include "scheduler.h"

#include <atomic>

#include "rodos.h"
#include "hw_specific.h"

namespace RODOS {

extern "C" {
  /**
  * A piece of memory used as stack by the scheduler.
  */
  char __schedulerStack__[SCHEDULER_STACKSIZE];

  /**
  * Address of the top of the scheduler's stack.
  * 8-byte aligned
  */
  long __schedulerStackFrame__ =
                  ((long)&(__schedulerStack__[SCHEDULER_STACKSIZE-sizeof(long)])) & (~7);
}

/** count all calls to the scheduler */
Uint64_Atomic_N_ThreadRO_1_InterruptRW Scheduler::scheduleCounter = 0;

Thread* Scheduler::preSelectedNextToRun = 0;
int64_t Scheduler::preSelectedEarliestSuspendedUntil = END_OF_TIME;


void schedulerWrapper(long* ctx) {
  Thread::currentThread.load()->context=ctx;
  Scheduler::schedule();
}

extern Thread* idlethreadP;
extern Int64_Atomic_N_ThreadRW_M_InterruptRW timeToTryAgainToSchedule;

/** activate idle thread */
void Scheduler::idle() {
  idlethreadP->suspendedUntil = 0;


  Thread::currentThread = idlethreadP;
  schedulerRunning = true;  /* a bit to early, but no later place possible */

  /* - the order of activate() and startIdleThread() is important -> don't change
   * - For all cortex ports a global context pointer is initialised in activate()
   *   and this must have been done before startIdleThread() is called.
   */
  int64_t nextSchedulingEventTime = timeToTryAgainToSchedule.load();
  int64_t nextTimeEventTime = END_OF_TIME;
#ifndef DISABLE_TIMEEVENTS
  nextTimeEventTime = TimeEvent::getNextTriggerTime();
#endif
  Timer::updateTriggerToNextTimingEvent(nextSchedulingEventTime, nextTimeEventTime);
  idlethreadP->activate();

  startIdleThread(); // only for some architectures, most implementations == nop()
  
}


void Scheduler::schedule() {
    // increment the schedule counter
    scheduleCounter.store(scheduleCounter.load() + 1);

    // time events to call?
    // now obsolete! call directly from timer!! TimeEvent::propagate(timeNow);

    // select the next thread to run: Do we have a preselection from Thread::yield()?
    Thread* nextThreadToRun = preSelectedNextToRun; // eventually set by Thread::yield()
    int64_t selectedEarliestSuspendedUntil = preSelectedEarliestSuspendedUntil;

    // in case we don't already have a preselected thread to run
    // -> actually do the work of finding the next thread in the schedule
    if(nextThreadToRun == 0) {
        nextThreadToRun = Thread::findNextToRun(selectedEarliestSuspendedUntil);
    }
    // use preselection only once
    preSelectedNextToRun = 0;

    // check if selected thread has stack violations (if yes -> switch to idleThread)
    bool hasStackViolations = nextThreadToRun->checkStackViolations();
    if(hasStackViolations) {
        nextThreadToRun = idlethreadP;
    }

    // update the respective variables according to the schedule
    nextThreadToRun->lastActivation.store(Scheduler::scheduleCounter); // timeNow ?? but what with on-os_xx, on-posix, etc?
    // it is important to set timeToTryAgainToSchedule as late as possible,
    // because we don't want to (potentially) steal time from our thread's next time slice
    // -> if the thread gets the whole time slice and updating the next SysTick succeeds,
    //    we only "waste" the thread's time for a small constant time period
    int64_t nextTimeEventTime = END_OF_TIME;
#ifndef DISABLE_TIMEEVENTS
    nextTimeEventTime = TimeEvent::getNextTriggerTime();
#endif
    int64_t nextTimeSliceEnd = TIME_SLICE_FOR_SAME_PRIORITY + NOW();
    // store raw without disabling interrupts, safe because:
    // -> timeToTryAgainToSchedule only read in SysTick interrupt (+ safely in yield)
    // -> interrupts disabled for scheduler in most ports (don't enable them prematurely)
    timeToTryAgainToSchedule.raw().store(min(selectedEarliestSuspendedUntil, nextTimeSliceEnd));

    // update SysTick timer to next event and jump into selected thread
    Timer::updateTriggerToNextTimingEvent(timeToTryAgainToSchedule.load(), nextTimeEventTime);
    nextThreadToRun->activate();
}

uint64_t Scheduler::getScheduleCounter() {
    return scheduleCounter.load();
}

} // namespace

