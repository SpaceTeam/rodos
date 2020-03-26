#include "rodos.h"

uint32_t printfMask = 0;

Topic<long> counter1(-1, "counter1");

static Application senderName("Publisher 01 simple", 1100);

class MyPublisher01 : public StaticThread<> {
  public:
    MyPublisher01() : StaticThread<>("SenderSimple") {}

    void run() {
        printfMask = 1;
        long cnt = 0;
        TIME_LOOP(0.03 * SECONDS, 0.03 * SECONDS) {
            if (cnt > 10) {
                hwResetAndReboot();
            }
            PRINTF("Publisher01 sending Counter1 %ld\n", ++cnt);
            counter1.publish(cnt);
        }
    }
} publisher01;
 

static Application reciverSimple("ReciverSimple", 1100);

class SimpleSub : public Subscriber {
  public:
    SimpleSub() : Subscriber(counter1, "simplesub") {}
    long put(const long topicId, const long len, const void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("SimpleSub - Length: %ld Data: %ld TopicId: %ld \n", len, *(long*)data, topicId);
        return 1;
    }
} simpleSub;
