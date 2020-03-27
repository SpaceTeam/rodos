#include "rodos.h"
#include "activity.h"

uint32_t printfMask = 0;

class A3 : public Activity {
public:
    A3() : Activity("A") { }
    void init() { activatePeriodic(0.1*SECONDS, 0.1*SECONDS); }
    void step(int64_t timeNow); 
} a3;

void A3::step(int64_t timeNow) {
    printfMask = 1;
    GOTO_LAST_YIELD;
    PRINTF("A1\n");
    YIELD_UNTIL(NOW() + 0.01*SECONDS);
    PRINTF("A2\n");
    YIELD_UNTIL(NOW() + 0.01*SECONDS);
    PRINTF("A3\n");
    YIELD_UNTIL(NOW() + 0.01*SECONDS);
    PRINTF("A4\n");
    GOTO_LAST_YIELD_END;
}

class A4 : public Activity {
public:
    A4() : Activity("A4") { }
    void init() { activateAt(0.11*SECONDS); }
    void step(int64_t timeNow); 
} a4;

void A4::step(int64_t timeNow) {
    printfMask = 1;
    static int cnt = 0;
    cnt++;
    if (cnt > 30) {
        PRINTF("----------------------------------------------------------- end now\n");
        hwResetAndReboot();
    }
    
    GOTO_LAST_YIELD;
    PRINTF("B Initialisation\n");
    while(1) {
        PRINTF("B1\n");
        YIELD_UNTIL(NOW() + 0.01*SECONDS);        
        PRINTF("B2\n");
        YIELD_UNTIL(NOW() + 0.01*SECONDS);
        PRINTF("B3\n");
        YIELD_UNTIL(NOW() + 0.01*SECONDS);
    }
    GOTO_LAST_YIELD_END;
}

