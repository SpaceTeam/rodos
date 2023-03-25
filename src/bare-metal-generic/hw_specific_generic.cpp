#include "hw_specific.h"

#include "default-platform-parameter.h"
#include "misc-rodos-funcs.h"
#include "timeevent.h"
#include "timemodel.h"

namespace RODOS {

extern long long timeToTryAgainToSchedule;

void Timer::setTimerIntervalToNextSystemEvent() {
    auto reactivationTime   = RODOS::min(timeToTryAgainToSchedule, TimeEvent::getNextTriggerTime());
    auto intervalInNanoSecs = RODOS::max(reactivationTime - NOW(), MIN_SYS_TICK_SPACING);
    Timer::setInterval(intervalInNanoSecs / 1000l); // nanoseconds to microseconds
}

} // namespace RODOS
