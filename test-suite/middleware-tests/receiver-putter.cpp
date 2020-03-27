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
 

static Application receiverName("ReciverPutter", 1100);

class JustPrint : public Putter {

    /// Implements the complete generic interface of putter
    bool putGeneric(const long topicId, unsigned int msgLen, const void* msg, const NetMsgInfo& netMsgInfo) {
        PRINTF("ReceiverPutter -  msgLen: %d data: %ld topicId: %ld\n", msgLen, *(long*)msg, topicId);
        return true;
    }

} justPrint;

static Subscriber nameNotImportant01(counter1, justPrint, "justprint01");
