#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTimebeats & events 02");


/** A time event to resume a thread **/

class Resumer : public TimeEvent {
    Thread* toBeResumed;

  public:
    Resumer() { toBeResumed = 0; }

    void resumeThreadAt(Thread* who, const int64_t when) {
        toBeResumed = who;
        activateAt(when);
    }

    void handle() {
        if(toBeResumed) toBeResumed->resume();
    }
};
static Resumer resumer;

/** A thread which has to be resumed by some one else */

class TestTimeBeat : public Thread {
  public:
    void run() {
        for (int i = 0; i < 10; i++) {
            resumer.resumeThreadAt(this, NOW() + 30 * MILLISECONDS);
            int32_t t0 = CNT_SECONDS_NOW();
            PRINTF("Timenow =  %d\n", t0);
            suspendCallerUntil();
        }
        hwResetAndReboot();
    }
};
static TestTimeBeat testTimeBeat;
