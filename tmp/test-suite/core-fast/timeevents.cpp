/**
* @file timeevents.cc
* @date 2008/06/11 16:07
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief test of time event management
*/

#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module01("TestTimeEvents");

class TimeEventTest : public TimeEvent {
  protected:
    void handle() {
        xprintf("SINGLE Time Event at %3.9f\n", CNT_SECONDS_NOW());
    }

    void init() {
        activateAt(25 * MILLISECONDS);
    }
};

static TimeEventTest te01;

class TimeEventTestPeriodic : public TimeEvent {
  public:
    void handle() {
        xprintf("Periodic Time Event at %3.9f\n", CNT_SECONDS_NOW());
    }

    void init() {
        activatePeriodic(0.04 * SECONDS, 10 * MILLISECONDS);
    }
};

static TimeEventTestPeriodic te02;

class TimeEventTest3 : public TimeEvent {
    int i = 0;
  protected:
    void handle() {
        i++;
        if (i > 10) {
            PRINTF("Terminating\n");
            FFLUSH();
            hwResetAndReboot();
        }
        xprintf("activateIn Time Event at %3.9f, reactivation in 0.02 seconds\n", CNT_SECONDS_NOW());
        activateAt(NOW() + 0.02 * SECONDS);
    }

    void init() {
        xprintf("activateIn( 12*MILLISECONDS) now=%3.9f\n", CNT_SECONDS_NOW());
        activateAt(NOW() + 12 * MILLISECONDS);
    }
};

static TimeEventTest3 te03;
