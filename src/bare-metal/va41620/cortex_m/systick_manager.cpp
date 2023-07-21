#include "systick_manager.h"

#include <atomic>

#include "cortex_m/peripheral_defs.h"
#include "hw_specific.h"
#include "timemodel.h"
#include "timeevent.h"

namespace RODOS {

static constexpr int64_t requiredTicksForTimerInterval = SysTickManager::calculateTicks(PARAM_TIMER_INTERVAL);

static_assert(requiredTicksForTimerInterval<SysTickManager::maxTicks && requiredTicksForTimerInterval>0,
                    "PARAM_TIMER_INTERVAL not possible with current clock config");

constinit uint32_t SysTickManager::ticks { 0 };

inline void SysTickManager::enable(){
    using namespace SysTick_CSR;
    systick.CSR.write(ENABLE(1),TICKINT(1));
}

void SysTickManager::stop(){
    using namespace SysTick_CSR;
    systick.CSR.write(ENABLE(0));
}

inline void SysTickManager::setReloadValue(uint32_t ticks){
    systick.RVR.write(ticks);
}

inline void SysTickManager::resetCurrentVal(){
    systick.CVR.write(0ul);
}

void SysTickManager::setInterval(const int64_t microseconds){
    int64_t requiredTicks = calculateTicks(microseconds);
    ticks = (requiredTicks>maxTicks) ? maxTicks : static_cast<uint32_t>(requiredTicks);
}

void SysTickManager::start(){
    stop();
    setReloadValue(ticks);
    resetCurrentVal();
    enable();
}


extern InterruptSyncWrapper<int64_t> timeToTryAgainToSchedule;
extern std::atomic<bool> yieldSchedulingLock;


extern "C" void SysTick_Handler() {

    if(yieldSchedulingLock) {
        return;
    }

#ifndef DISABLE_TIMEEVENTS
    TimeEvent::propagate(NOW());
#endif

    // if not time yet to schedule (SysTick only triggered for TimeEvent) return directly
    if(NOW() < timeToTryAgainToSchedule) {
        int64_t nextSchedulingEventTime = timeToTryAgainToSchedule.load();
        int64_t nextTimeEventTime = TimeEvent::getNextTriggerTime();
        Timer::updateTriggerToNextTimingEvent(nextSchedulingEventTime, nextTimeEventTime);
        Timer::start();
        return;
    }

    __asmSaveContextAndCallScheduler();
}

}
