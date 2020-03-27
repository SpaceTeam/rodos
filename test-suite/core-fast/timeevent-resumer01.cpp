#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTimebeats & events 01");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        PRINTF("First beat in 0.05 seconds, period 0.05 seconds\n");
        int i = 0;
        TIME_LOOP(0.05*SECONDS, 0.05*SECONDS) {
            i++;
            if (i > 10) {
                hwResetAndReboot();
            }
            int32_t t0 = CNT_SECONDS_NOW();
            PRINTF("In 0.05 seconds beat: %d \n", t0);
        }
    }
};
static TestTimeBeat testTimeBeat;


class Resumer : public TimeEvent {
  protected:
    void handle() {
        int32_t t0 = CNT_SECONDS_NOW();
        xprintf("Time Event at %d -> resuming\n", t0);
        testTimeBeat.resume();
        if(NOW() < 50 * MILLISECONDS) activateAt(NOW() + 0.02*SECONDS);
    }

    void init() {
        activateAt(10 * SECONDS);
    }
};
static Resumer resumer;
