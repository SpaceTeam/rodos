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

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg = static_cast<const GenericMsgRef*>(data);
        xprintf("\n Async: %d %s\n", (int)msg->msgLen, msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
