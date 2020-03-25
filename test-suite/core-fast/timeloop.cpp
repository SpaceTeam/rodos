#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTimebeats02");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        PRINTF("First beat in 0.03 seconds, period 0.02 seconds\n");
        int i = 0;

        TIME_LOOP(0.03 * SECONDS, 20 * MILLISECONDS) {
            PRINTF("in 0.02 seconds beat: %3.9f\n", INT_SECONDS_NOW());
            i++;
            if (i == 10) {
                hwResetAndReboot();
            }
        }
    }
};


static TestTimeBeat testTimeBeat;
