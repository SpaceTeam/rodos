#include "rodos.h"

#ifndef TIME_TO_END
#define TIME_TO_END 40
#endif

#include "prt-seconds-now.h"
int32_t secondsNowCnt = 1;

static Application terminateTestApp("terminateTest");

class TerminateTest : public Thread {
  public:
    TerminateTest() : Thread("TerminateTest", CEILING_PRIORITY) {}
    void run() {
        PRINTF("\nThis run (test) will be terminated in %d Seconds\n", TIME_TO_END);
        AT(TIME_TO_END*SECONDS);
        PRINTF("\nThis run (test) terminates now! but first error log:\n");
        errorLog.printAll();
        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();
    }
};

static TerminateTest terminateTest;


//____________________________________________________

namespace RODOS {
extern long myNodeNr;
}

static class SetNodeNr : public Initiator {
    void init() {
        RODOS::myNodeNr = 7;
        xprintf(" initiator nodnr = %ld\n", RODOS::myNodeNr);
    }
} setNodeNr;


