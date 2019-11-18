#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class HighPriorityThread01 : public Thread {
  public:
    HighPriorityThread01() : Thread("HiPriority", 25) {
    }

    void init() {
        PRINTF(" hipri = '*'");
    }

    void run() {
        for (int i = 0; i < 100; i++) {
            PRINTF("*");
            FFLUSH();
            suspendCallerUntil();
        }
    }
};

HighPriorityThread01 highPriorityThread01;


class LowPriorityThread : public Thread {
  public:
    LowPriorityThread() : Thread("LowPriority", 10) {
    }

    void init() {
        PRINTF(" lopri = '.'");
    }

    void run() {
        long long cnt = 0;
        for (int i = 0; i < 200; i++) {
            cnt++;
            if(cnt % 5 == 0) {
                PRINTF(".");
                FFLUSH();
            }
            if(cnt % 11 == 0) {
                highPriorityThread01.resume();
            }
        }
        hwResetAndReboot();
    }
};


/******************/

LowPriorityThread lowPriorityThread;

/******************/
