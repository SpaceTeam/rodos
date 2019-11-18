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
 

static Application receiverName("ReciverComBuffer", 1200);

static CommBuffer<long> buf;
static Subscriber       receiverBuf(counter1, buf, "receiverbuf");

class ReceiverBuf : public Thread {
    void run() {
        long cnt;
        TIME_LOOP(0, 0.011 * SECONDS) {
            buf.get(cnt);
            PRINTF("ReceiverComBuffer - counter1: %ld\n", cnt);
        }
    }
} recbuf;
