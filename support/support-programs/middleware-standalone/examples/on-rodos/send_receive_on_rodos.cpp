#include "rodos.h"
#include "gateway.h"

Topic<long> counter5(1002, "counter5"); // to receive (subscrived)
Topic<long> counter6(1003, "counter6"); // to send (pulish)

/******************************/

static UDPInOut udp(-50000);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway gateway1(&linkinterfaceUDP, true);

/******************************/

class MyPublisher16 : public StaticThread<> {
public:
  MyPublisher16() : StaticThread("sender16") {}
  void run() {
    long cnt = 10000;
    TIME_LOOP(0, 1600 * MILLISECONDS) {
      PRINTF("sending %ld\n", --cnt);
      counter6.publish(cnt);
    }
  }
} myPublisher16;

struct Receiver : public Subscriber {
  Receiver() : Subscriber(counter5, "justprint11") {}
  uint32_t put(const uint32_t topicId, const size_t len, void *msg, const NetMsgInfo &) {

    PRINTF("Got len=%d, topicId=%d Msg-Data %ld \n", (int)len, topicId,  *(long *)msg);
    return true;
  }
} justPrint11;
