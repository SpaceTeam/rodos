#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module02("TestTimeAT");

static class TestTime : public Thread {

  public:
    TestTime() : Thread("waitAT") {}
    void run() {
        int cnt = 0;

        PRINTF("Starting and waiting until 3th second\n");
        AT(3 * SECONDS);
        PRINTF("after 3th second\n");

        while(1) {
            cnt++;
            AT(NOW() + 2 * SECONDS);
            PRINTF("After 2 Seconds  : %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
    }

    void init() { PRINTF("Waiting 2 seconds"); }
} testTime;
