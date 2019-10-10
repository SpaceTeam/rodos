#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTimebeats02");

class TestTimeBeat : public Thread {
  public:
    void run() {
        PRINTF("First beat in 3 seconds, period 2 seconds\n");

        TIME_LOOP(3 * SECONDS, 2 * SECONDS) {
            PRINTF("in 2 seconds beat: %3.9f\n", CNT_SECONDS_NOW());
        }
    }
};


static TestTimeBeat testTimeBeat;
