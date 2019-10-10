#include "rodos.h"

#include "../prt-seconds-now.h"


static Application module02("TestTimeAT");

static class TestTime : public Thread {

  public:
    TestTime() : Thread("waitAT") {}

    void run() {

        PRINTF("waiting until 3rd second after start\n");
        AT(1 * SECONDS);
        PRINTF("after 3rd second\n");

        PRINTF("waiting until 1 second has pased\n");
        AT(NOW() + 1 * SECONDS);
        PRINTF("1 second has pased\n");

        PRINTF("print every 2 seconds, start at 5 seconds\n");
        TIME_LOOP(3 * SECONDS, 200 * MILLISECONDS) {
            PRINTF("current time: %3.9f\n", CNT_SECONDS_NOW());
        }
    }

    void init() { PRINTF("Waiting time"); }

} testTime;
