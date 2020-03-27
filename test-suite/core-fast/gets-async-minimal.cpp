#include "rodos.h"

uint32_t printfMask = 0;

void MAIN() {
    printfMask = 1;
    PRINTF("please write some lines\n");
    activateTopicCharInput(); //<........ THIS IS IT!!!!
    hwResetAndReboot();
}


class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt(const long topicId, const void* data, int len) {
        GenericMsgRef* msg = (GenericMsgRef*)data;
        xprintf("\n Async: %d %s\n", (int)msg->msgLen, msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
