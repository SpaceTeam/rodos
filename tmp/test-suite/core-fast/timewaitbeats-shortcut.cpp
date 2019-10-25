#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTimebeats02");

class TestTimeBeat : public Thread {
  public:
    void run() {
        PRINTF("First beat in 0.03 seconds, period 0.02 seconds\n");

        int cnt = 0;
        
        TIME_LOOP(0.03 * SECONDS, 0.02 * SECONDS) {
            PRINTF("in 0.02 seconds beat: %3.9f\n", CNT_SECONDS_NOW());
            cnt++;
            if (cnt > 10) {
                hwResetAndReboot();
            }
        }
    }
};


static TestTimeBeat testTimeBeat;
