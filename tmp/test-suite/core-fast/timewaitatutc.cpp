#include "rodos.h"

static Application module02("TestTimeAT");

static class TestTime : public Thread {

  public:
    TestTime() : Thread("waitAT") {}
    void run() {
        sysTime.setUTC(100 * SECONDS);
        PRINTF("Starting and waiting until 1st second\n");
        AT_UTC(101 * SECONDS);
        PRINTF("after 1st second\n");
        hwResetAndReboot();
    }
} testTime;
