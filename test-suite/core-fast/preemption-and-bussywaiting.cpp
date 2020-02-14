#include "rodos.h"


class LowPriorityThread : public Thread {
  public:
    LowPriorityThread() : Thread("LowPriority", 100) {}

    void run() {
        volatile int64_t cnt             = 0;
        int64_t          intervalToPrint = 1000000 * 10;
        PRINTF("\nLow prio begins printing '.'\n");
        while(1) {
            if(++cnt % intervalToPrint == 0) {
                PRINTF(".");
                FFLUSH();
                AT(NOW() + 10 * MILLISECONDS);
            }
        }
    }
} lowPrio;

//__________________________________________________________

class HiPriorityThread : public Thread {
  public:
    HiPriorityThread() : Thread("HiPriority", 200) {}

    void run() {
        volatile int64_t cnt             = 0;
        int64_t   intervalToPrint = getSpeedKiloLoopsPerSecond() * 10;
        PRINTF("\nHi prio waits 1 second and then print +\n");
        AT(1 * SECONDS);
        PRINTF("\nHi Priority in busy waiting\n");
        for (int64_t i=0; i < intervalToPrint*20; i++) {
            if(++cnt % intervalToPrint == 0) {
                PRINTF("+");
                FFLUSH();
            }
        }


        PRINTF("\nHi prio in busy waiting in PRIORITY_FLOORING\n");
        {
            PRIORITY_FLOORING_IN_SCOPE();
            for (int64_t i=0; i < intervalToPrint*20; i++) {
                if(++cnt % intervalToPrint == 0) {
                    PRINTF("+");
                    FFLUSH();
                }
            }
        }
        PRINTF("\nHi prio Ends\n");
        hwResetAndReboot();
    }
} hiPri;
