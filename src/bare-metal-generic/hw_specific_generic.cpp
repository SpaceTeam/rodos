#include "hw_specific.h"

#include "default-platform-parameter.h"
#include "misc-rodos-funcs.h"
#include "interrupt_sync.h"
#include "timemodel.h"

namespace RODOS {

extern InterruptSyncWrapper<int64_t> timeToTryAgainToSchedule;

void Timer::updateTriggerToNextTimingEvent(int64_t nextTriggerTime) {
#ifndef DISABLE_TIMEEVENTS
    int64_t reactivationTime = min(timeToTryAgainToSchedule.load(), nextTriggerTime);
#else
    int64_t reactivationTime = timeToTryAgainToSchedule.load();
#endif
    // don't set interval to less than MIN_SYS_TICK_SPACING
    // -> this is done to avoid flooding the system with SysTick interrupts
    // -> deadlines may be overrun by maximally:
    //    MIN_SYS_TICK_SPACING (+ scheduling overhead + accumulated previous overrunning)
    int64_t intervalInNanoSecs = max(reactivationTime - NOW(), MIN_SYS_TICK_SPACING);
    Timer::setInterval(intervalInNanoSecs / 1000l); // nanoseconds to microseconds
}

} // namespace RODOS
