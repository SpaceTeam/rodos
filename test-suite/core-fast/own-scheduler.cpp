#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTime");

class TestTime : public Thread {
  public:
    TestTime() : Thread("waitfor") {}
    void run() {
        int cnt = 0;
        while(1) {
            cnt++;
            suspendCallerUntil();
            PRINTF(" %x, After 0.02 Seconds  : %3.9f %d\n", (int)((int64_t)this & 0x0fffffff), CNT_SECONDS_NOW(), cnt);
        }
    }
};


TestTime t[4];


class MySchedluler : public Thread {
  public:
    MySchedluler() : Thread("mySchedluler", 10) {}
    void run() {
        int cnt = 0;
        TIME_LOOP(0.1 * SECONDS, 0.02 * SECONDS) {
            cnt++;
            if (cnt > 25) {
                hwResetAndReboot();
            }
            PRINTF("-----------------------------------\n");
            for(int i = 0; i < 4; i++) {
                t[i].resume();
                yield();
            }
        }
    }
} mySchelduer;