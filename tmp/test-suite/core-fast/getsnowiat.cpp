#include "rodos.h"

static Application module01("Testgetchar");

class TestGets : public Thread {
  public:
    TestGets() : Thread("testgetchar") {}
    void run() {
        int cnt = 0;
        char* s;
        PRINTF("Please type string of characters\n");
        TIME_LOOP(0.01 * SECONDS, 0.01 * SECONDS) {
            cnt++;
            if (cnt > 10) {
                PRINTF("-------------------------------------------------------------");
                hwResetAndReboot();
            }
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: %s'%s'%s **********\n", SCREEN_GREEN, s, SCREEN_RESET); }
            PRINTF("Waiting 0.01 Seconds\n");
        }
    }
} testGets;
