#include "rodos.h"

uint32_t printfMask = 0;

/****** Use moduleId 2000 just be be found by other example: ceiler **/
static Application module01("PreemptionTest", 2000);

class HighPriorityThread : public StaticThread<> {
  public:
    static constexpr auto PRINT_INTERVAL = 500 * MILLISECONDS;

    HighPriorityThread() : StaticThread<>("HiPriority", 25) {
    }

    void init() {
        printfMask = 1;
        xprintf(" hipri  = '*'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
        TIME_LOOP(0, PRINT_INTERVAL) {
            xprintf("*");
            FFLUSH();
        }
    }
};


class LowPriorityThread : public StaticThread<> {
  public:
    static constexpr auto PRINT_INTERVAL   = 50 * MILLISECONDS;
    static constexpr auto PRINTF_WAIT_TIME = 5 * MILLISECONDS;
    static constexpr auto WAIT_JITTER      = 2 * MILLISECONDS;

    LowPriorityThread() : StaticThread<>("LowPriority", 10) {
    }

    void init() {
        printfMask = 1;
        xprintf(" lopri  = '.'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;

        for(auto nextTick = PRINTF_WAIT_TIME; true;) {
            if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                xprintf(".");
                FFLUSH();
            }
            nextTick = TimeModel::computeNextBeat(PRINTF_WAIT_TIME, PRINT_INTERVAL, NOW());
            busyWaitUntil(nextTick);
        }
    }
};


/******************/

static Application module02("PriorityCeiling", 3000);

class PriorityCeiler : public StaticThread<> {
  public:
    static constexpr auto PRINT_INTERVAL   = 50 * MILLISECONDS;
    static constexpr auto PRINTF_WAIT_TIME = 10 * MILLISECONDS;
    static constexpr auto WAIT_JITTER      = 2 * MILLISECONDS;

    static constexpr auto INIT_SUSPEND_TIME = 3 * SECONDS;
    static constexpr auto BASE_PRINTF_BEAT  = INIT_SUSPEND_TIME + PRINTF_WAIT_TIME;

    static constexpr auto START_LOOP_1          = INIT_SUSPEND_TIME;
    static constexpr auto DURATION_LOOP_1       = 5 * SECONDS;
    static constexpr auto START_LOOP_2          = START_LOOP_1 + DURATION_LOOP_1;
    static constexpr auto DURATION_LOOP_2       = 3 * SECONDS;
    static constexpr auto START_LOOP_3          = START_LOOP_2 + DURATION_LOOP_2;
    static constexpr auto DURATION_LOOP_3       = 5 * SECONDS;
    static constexpr auto START_LOOP_4          = START_LOOP_3 + DURATION_LOOP_3;
    static constexpr auto DURATION_LOOP_4       = 3 * SECONDS;
    static constexpr auto START_LAST_SUSPEND    = START_LOOP_4 + DURATION_LOOP_4;
    static constexpr auto DURATION_LAST_SUSPEND = 2 * SECONDS;

    PriorityCeiler() : StaticThread<>("PriorityCeiler", 10) {
    }

    void init() {
        printfMask = 1;
        xprintf(" ceiler = '-'\n");
        printfMask = 0;
    }


    void run() {
        printfMask = 1;

        suspendCallerUntil(INIT_SUSPEND_TIME + WAIT_JITTER);

        xprintf("\nPriorityCeiler: %llds normal\n", DURATION_LOOP_1 / SECONDS);
        for(auto nextTick = START_LOOP_1 + PRINTF_WAIT_TIME; true;) {
            if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                xprintf("-");
                FFLUSH();
            }
            if(NOW() >= START_LOOP_1 + DURATION_LOOP_1) {
                break;
            }
            nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
            busyWaitUntil(nextTick);
        }

        xprintf("\nPriorityCeiler: %llds ceiling\n", DURATION_LOOP_2 / SECONDS);
        {
            PRIORITY_CEILER_IN_SCOPE();

            for(auto nextTick = START_LOOP_2 + PRINTF_WAIT_TIME; true;) {
                if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                    xprintf("-");
                    FFLUSH();
                }
                if(NOW() >= START_LOOP_2 + DURATION_LOOP_2) {
                    break;
                }
                nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
                busyWaitUntil(nextTick);
            }
        }

        xprintf("\nPriorityCeiler: %llds normal\n", DURATION_LOOP_3 / SECONDS);
        for(auto nextTick = START_LOOP_3 + PRINTF_WAIT_TIME; true;) {
            if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                xprintf("-");
                FFLUSH();
            }
            if(NOW() >= START_LOOP_3 + DURATION_LOOP_3) {
                break;
            }
            nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
            busyWaitUntil(nextTick);
        }

        xprintf("\nPriorityCeiler: %llds normal + suspending for %llds\n",
                DURATION_LOOP_4 / SECONDS,
                DURATION_LAST_SUSPEND / SECONDS);
        for(auto nextTick = START_LOOP_4 + PRINTF_WAIT_TIME; true;) {
            if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                xprintf("-");
                FFLUSH();
            }
            if(NOW() >= START_LOOP_4 + DURATION_LOOP_4) {
                break;
            }
            nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
            busyWaitUntil(nextTick);
        }

        suspendCallerUntil(START_LAST_SUSPEND + DURATION_LAST_SUSPEND);

        xprintf("\nTest terminates\n");
        hwResetAndReboot();
    }
};


/******************/

/**
 * @name Order of instantiation matters
 * (e.g. PriorityCeiler must be scheduled before LowPriorityThread initially)
 * @{
 */
HighPriorityThread highPriorityThread;
LowPriorityThread  lowPriorityThread;
PriorityCeiler     priorityCeiler;
/** @} */

/******************/

void MAIN() {
    printfMask = 1;
    if(Application::findApplication(2000) == 0) {
        xprintf("\n\n**********************\n");
        xprintf("To be able to test it compile together with PreemptionTest\n");
        xprintf("**********************\n\n\n");
    } else {
        xprintf("PreemptionTest found\n");
    }
}
