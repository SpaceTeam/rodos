#include "rodos.h"
#include "activity.h"

#include "../prt-seconds-now.h"

static int cnt = 0;

class A1 : public Activity {
public:
    A1() : Activity("A1", 200, 0.05*SECONDS, 0.05*SECONDS) { }
    void step(int64_t timeNow) {
        PRINTF("A1 %4.6f\n", CNT_SECONDS_NOW());
        cnt++;
        if (cnt > 10) {
            hwResetAndReboot();
        }
    }
} a1;

class A2 : public Activity {
public:
    A2() : Activity("A2") { }
    void init() { activatePeriodic(0.02*SECONDS, 0.03*SECONDS); }
    void step(int64_t timeNow) { PRINTF("A2 %4.6f\n", CNT_SECONDS_NOW());}
} a2;

