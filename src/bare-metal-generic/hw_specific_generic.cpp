#include "hw_specific.h"

#include "default-platform-parameter.h"
#include "misc-rodos-funcs.h"
#include "timeevent.h"
#include "timemodel.h"

namespace RODOS {

extern long long timeToTryAgainToSchedule;

void Timer::updateTriggerToNextTimingEvent() {
    auto nextTriggerTime = TimeEvent::getNextTriggerTime();
    auto timeNow         = NOW();
    if(nextTriggerTime < timeNow) {
        TimeEvent::propagate(timeNow);
    }
    auto reactivationTime   = RODOS::min(timeToTryAgainToSchedule, nextTriggerTime);
    auto intervalInNanoSecs = RODOS::max(reactivationTime - NOW(), MIN_SYS_TICK_SPACING);
    Timer::setInterval(intervalInNanoSecs / 1000l); // nanoseconds to microseconds
}

} // namespace RODOS
