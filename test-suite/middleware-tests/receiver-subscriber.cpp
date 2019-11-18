#include "rodos.h"

Topic<long> counter1(-1, "counter1");

static Application senderName("Publisher 01 simple", 1100);

class MyPublisher01 : public Thread {
  public:
    MyPublisher01() : Thread("SenderSimple") {}

    void run() {
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
    long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
        PRINTF("SimpleSub - Length: %ld Data: %ld TopicId: %ld \n", len, *(long*)data, topicId);
        return 1;
    }
} simpleSub;
