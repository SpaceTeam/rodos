#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("resumefromEvent");

class TestWaiter : public Thread {
  public:
    void run() {
        for (int i = 0; i < 10; i++) {
            PRINTF("Suspend and wait until some one resumes me\n");
            suspendCallerUntil();
            //double t0 = SECONDS_NOW();
            PRINTF("testwaiter running again at %3.9f\n", CNT_SECONDS_NOW());
        }
        hwResetAndReboot();
    }
};


static TestWaiter testWaiter;


class TimeEventTest : public TimeEvent {
  public:
    void handle() {
        xprintf("   Time Event at %3.9f\n", CNT_SECONDS_NOW());
        testWaiter.resume();
        xprintf("   Testwaiter resumed from me\n");
    }

    void init() { activatePeriodic(0.05 * SECONDS, 0.03 * SECONDS); }
};

static TimeEventTest te01;
