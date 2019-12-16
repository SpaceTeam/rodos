
#include "rodos.h"

#include "../prt-seconds-now.h"

Semaphore onlyOne;
int       globalCnt = 0;

class Watcher : public Thread {
  public:
    void run() {
        onlyOne.enter();
        int myId = globalCnt++;
        onlyOne.leave();
        
        int i = 0;

        TIME_LOOP(0.03 * SECONDS, 0.02 * SECONDS) {
            i++;
            if (i > 5) {
                hwResetAndReboot();
            }
            onlyOne.enter();
            PRINTF(" only one, I am -- %02d -- ,", myId);
            yield(); // Just to show that the semaphore protects
            PRINTF("time %3.9f\n", CNT_SECONDS_NOW());
            onlyOne.leave();
        }
        
    }
} team[20];