#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module02("TestTimeAT");

static class TestTime : public Thread {

  public:
    TestTime() : Thread("waitAT") {}
    void run() {
        int cnt = 0;

        PRINTF("Starting and waiting until 0.03 second\n");
        AT(0.03 * SECONDS);
        PRINTF("after 0.03 second\n");

        for (int i = 0; i < 10; i++) {
            cnt++;
            AT(NOW() + 0.02 * SECONDS);
            PRINTF("After 0.02 Seconds  : %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
        hwResetAndReboot();
    }

    void init() { PRINTF("Waiting 2 seconds"); }
} testTime;
