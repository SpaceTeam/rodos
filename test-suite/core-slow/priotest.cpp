#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

#define LOOPS 100000000

static Application module01("priorityTest", 3000);


class HiPriorityThread : public Thread {
  public:
    HiPriorityThread() : Thread("HiPriority", 100) {
    }

    void init() {
        xprintf(" hipri");
    }

    void run() {
        long long loopCnt = 0;
        long long cnt     = 0;
        while(cnt < LOOPS * 10) {
            cnt++;
            if(cnt % LOOPS == 0) {
                loopCnt++;
                PRINTF("  hi pri cnt = %lld\n", loopCnt);
            }
        }
        hwResetAndReboot();
    }
} hipri1, hipri2;


/******************/

class LowPriorityThread : public Thread {
  public:
    LowPriorityThread() : Thread("LowPriority", 30) {
    }

    void init() {
        xprintf(" lopri");
    }

    void run() {
        long long loopCnt = 0;
        long long cnt     = 0;
        while(cnt < LOOPS * 10) {
            cnt++;
            if(cnt % LOOPS == 0) {
                loopCnt++;
                PRINTF(" low pri cnt = %lld\n", loopCnt);
            }
        }
    }
} lopri1, lopri2;
