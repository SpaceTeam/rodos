#include "rodos.h"

static Application module01("TestTimebeats & events 01");

class TestTimeBeat : public Thread {
  public:
    void run() {
        PRINTF("First beat in 5 seconds, period 5 seconds\n");
        TIME_LOOP(5*SECONDS, 5*SECONDS) {
            int32_t t0 = SECONDS_NOW();
            PRINTF("in 5 seconds beat: %d \n", t0);
        }
    }
};
static TestTimeBeat testTimeBeat;


class Resumer : public TimeEvent {
  protected:
    void handle() {
        int32_t t0 = SECONDS_NOW();
        xprintf("Time Event at %d -> resuming\n", t0);
        testTimeBeat.resume();
        if(SECONDS_NOW() < 50) activateAt(NOW() + 2*SECONDS);
    }

    void init() {
        activateAt(10 * SECONDS);
    }
};
static Resumer resumer;
