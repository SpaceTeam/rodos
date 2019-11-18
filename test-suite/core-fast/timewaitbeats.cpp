#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTime02");

class TestTimeBeat : public Thread {
  public:
    void run() {
        int cnt = 0;
        setPeriodicBeat(0.03 * SECONDS, 0.02 * SECONDS);
        PRINTF("First beat in 0.03 seconds, period 0.02 seconds\n");
        for (int i = 0; i < 10; i++) {
            cnt++;
            suspendUntilNextBeat();
            PRINTF("in 0.02 seconds beat: %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
        hwResetAndReboot();
    }
};


static TestTimeBeat testTimeBeat;
