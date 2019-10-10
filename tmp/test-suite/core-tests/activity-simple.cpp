#include "rodos.h"
#include "activity.h"

#include "../prt-seconds-now.h"

class A1 : public Activity {
public:
    A1() : Activity("A1", 200, 5*SECONDS, 5*SECONDS) { }
    void step(int64_t timeNow) { PRINTF("A1 %4.6f\n", CNT_SECONDS_NOW()); }
} a1;

class A2 : public Activity {
public:
    A2() : Activity("A2") { }
    void init() { activatePeriodic(2*SECONDS, 3*SECONDS); }
    void step(int64_t timeNow) { PRINTF("A2 %4.6f\n", CNT_SECONDS_NOW());}
} a2;

