#include "rodos.h"

static Application module01("Testgetchar");

class TestGets : public Thread {
  public:
    TestGets() : Thread("testgetchar") {}
    void run() {
        char* s;
        PRINTF("Please type string of characters. Run at least 0.04 seconds\n");

        activateTopicCharInput(); //<........ THIS IS IT!!!!

        int cnt = 0;
        
        TIME_LOOP(0.3 * SECONDS, 0.01 * SECONDS) {
            cnt++;
            if (cnt > 100) {
                PRINTF("-------------------------------------------------------------");
                hwResetAndReboot();
            }
            
            PRINTF("I call getsnowait -> topic deactivated! \n");
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: %s'%s'%s **********\n", SCREEN_GREEN, s, SCREEN_RESET); }
        }
    }
} testGets;

/******************************/

class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt(const long topicId, const void* data, int len) {
        GenericMsgRef* msg       = (GenericMsgRef*)data;
        msg->msgPtr[msg->msgLen] = 0;
        xprintf("\n Async: %d %s\n", (int)msg->msgLen, msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
