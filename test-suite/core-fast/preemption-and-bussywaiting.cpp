#include "rodos.h"

uint32_t printfMask = 0;

class LowPriorityThread : public StaticThread<> {
  public:
    static constexpr auto PRINT_INTERVAL = 10 * MILLISECONDS;
    static constexpr auto WAIT_JITTER    = 2 * MILLISECONDS;

    LowPriorityThread() : StaticThread<>("LowPriority", 100) {}

    void run() {
        printfMask = 1;
        PRINTF("\nLow prio begins printing '.'\n");

        while(true) {
            PRINTF(".");
            FFLUSH();

            Thread::suspendCallerUntil(TimeModel::computeNextBeat(0, PRINT_INTERVAL, NOW()));

            auto nextTick = TimeModel::computeNextBeat(0, PRINT_INTERVAL, NOW());
            while(NOW() < nextTick) {}
        }
    }
} lowPrio;

//__________________________________________________________

class HiPriorityThread : public StaticThread<> {
  public:
    static constexpr auto PRINT_INTERVAL = 10 * MILLISECONDS;

    /*
     * Wait period before printing is necessary to avoid deadlock of printf
     * -> if suspend of LowPriorityThread interrupts PRINTF of HiPriorityThread => deadlock
     * => solution: shift HiPriorityThread period a bit so that periods don't overlap
     */
    static constexpr auto PRINTF_WAIT_TIME = 4 * MILLISECONDS;
    static constexpr auto WAIT_JITTER      = 2 * MILLISECONDS;

    static constexpr auto INIT_WAIT_TIME   = 100 * MILLISECONDS;
    static constexpr auto BASE_PRINTF_BEAT = INIT_WAIT_TIME + PRINTF_WAIT_TIME;

    static constexpr auto START_LOOP_1    = INIT_WAIT_TIME;
    static constexpr auto DURATION_LOOP_1 = 200 * MILLISECONDS;

    static constexpr auto START_LOOP_2    = START_LOOP_1 + DURATION_LOOP_1;
    static constexpr auto DURATION_LOOP_2 = 400 * MILLISECONDS;

    HiPriorityThread() : StaticThread<>("HiPriority", 200) {}

    void run() {
        printfMask = 1;

        PRINTF("\nHi prio waits and then prints '+'\n");
        suspendCallerUntil(INIT_WAIT_TIME + WAIT_JITTER);

        PRINTF("\nHi prio in busy waiting\n");
        for(auto nextTick = START_LOOP_1 + PRINTF_WAIT_TIME; true;) {
            if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                xprintf("+");
                FFLUSH();
            }
            if(NOW() >= START_LOOP_1 + DURATION_LOOP_1) {
                break;
            }
            nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
            busyWaitUntil(nextTick);
        }

        /*
         * Hi prio in busy waiting in PRIORITY_FLOORING
         * (time diagram sketch, HiPriorityThread period shifted a bit):
         *
         * LowPriorityThread: L---xxxxL---xxxxL---xxxxL---
         * HiPriorityThread:  -HxxxHxxxHxxxHxxxHxxxHxxxHxxx
         * together:          LHxxxxxxLHxxxxxxLHxxxxxxLHxxx
         *
         * -> 'L/H': print character
         * -> 'x': busy waiting loop
         * -> '-': suspending
         */
        PRINTF("\nHi prio in busy waiting in PRIORITY_FLOORING\n");
        {
            PRIORITY_FLOORING_IN_SCOPE();
            yield();

            for(auto nextTick = START_LOOP_2 + PRINTF_WAIT_TIME; true;) {
                if(NOW() >= nextTick && NOW() <= nextTick + WAIT_JITTER) {
                    xprintf("+");
                    FFLUSH();
                }
                if(NOW() >= START_LOOP_2 + DURATION_LOOP_2) {
                    break;
                }
                nextTick = TimeModel::computeNextBeat(BASE_PRINTF_BEAT, PRINT_INTERVAL, NOW());
                busyWaitUntil(nextTick);
            }
        }
        PRINTF("\nHi prio ends\n");
        hwResetAndReboot();
    }
} hiPri;
