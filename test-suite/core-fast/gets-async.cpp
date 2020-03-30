#include "rodos.h"

uint32_t printfMask = 0;

static Application module01("Testgetchar");

class TestGets : public StaticThread<> {
  public:
    TestGets() : StaticThread<>("testgetchar") {}
    void run() {
        printfMask = 1;
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

    void putFromInterrupt([[gnu::unused]] const long topicId, const void* data, [[gnu::unused]] int len) {
        const GenericMsgRef* msg       = static_cast<const GenericMsgRef*>(data);
        msg->msgPtr[msg->msgLen] = 0;
        xprintf("\n Async: %d %s\n", (int)msg->msgLen, msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
