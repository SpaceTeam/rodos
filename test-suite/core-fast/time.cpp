#include "rodos.h"

#include "../prt-seconds-now.h"


static Application module02("TestTimeAT");

static class TestTime : public StaticThread<> {

  public:
    TestTime() : StaticThread<>("waitAT") {}

    void run() {

        PRINTF("waiting until 0.01 second after start\n");
        AT(0.01 * SECONDS);
        PRINTF("after 0.01 second\n");

        PRINTF("waiting until 0.015 second has passed\n");
        AT(NOW() + 0.015 * SECONDS);
        PRINTF("1 second has pased\n");

        PRINTF("print every 0.02 seconds, start at 0.04 seconds\n");
        
        int cnt = 0;
        
        TIME_LOOP(0.015 * SECONDS, 20 * MILLISECONDS) {
            PRINTF("current time: %3.9f\n", CNT_SECONDS_NOW());
            cnt++;
            if (cnt > 10) {
                hwResetAndReboot();
            }
        }
    }

    void init() { PRINTF("Waiting time"); }

} testTime;
