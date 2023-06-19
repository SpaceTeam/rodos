#include "rodos.h"
#include "demo_topics.h"

class Receiver1 : public SubscriberReceiver<myDataType> {
  public:
    Receiver1() : SubscriberReceiver<myDataType>(topic1, "receiver1") {}
    void put(myDataType& msg) {
        PRINTF("at %9.3f       receiver1 : got \t %s %d\n", SECONDS_NOW(), msg.stringMsg, msg.cnt);
    }
} simpleSub1;
