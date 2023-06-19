#include "rodos.h"
#include "demo_topics.h"

class Receiver2 : public SubscriberReceiver<myDataType> {
  public:
    Receiver2() : SubscriberReceiver<myDataType>(topic2, "receiver2") {}
    void put(myDataType& msg) {
        PRINTF("at %9.3f       receiver2 : got \t %s %d\n", SECONDS_NOW(), msg.stringMsg, msg.cnt);
    }
} simpleSub2;
