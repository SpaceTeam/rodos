#include "rodos.h"
#include "io-mask.h"

void MAIN() {
    INIT_MASK();
    activateTopicCharInput(); //<........ THIS IS IT!!!!
}

/***** outputs ********/
class ThreadAA : public Thread {
  public:
    void run() {
        int cnt = 0;
        TIME_LOOP(0.01 * SECONDS, 0.01 * SECONDS) { 
            PRINTF(CNTAA "%d" INPUTLINE, cnt++);
            if (cnt > 100) {
                hwResetAndReboot();
            }
        }
    }
} aa;


class ThreadBB : public Thread {
  public:
    void run() {
        int cnt = 0;
        TIME_LOOP(0.03 * SECONDS, 5 * MILLISECONDS) { PRINTF(CNTBB "%d" INPUTLINE, cnt++); }
    }
} bb;

/********* Input *********/

class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt(const long topicId, const void* data, int len) {
        GenericMsgRef* msg = (GenericMsgRef*)data;
        PRINTF(READ "                      ");
        PRINTF(READ "%s\n", msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
