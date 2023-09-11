#include "rodos.h"
#include "demo_topics.h"

myDataType msgToSend;

class MyPublisher : public StaticThread<> {
  public:
    MyPublisher() : StaticThread<>("sender") { }

    void run() {
        int32_t counter = 0;
        strcpy(msgToSend.stringMsg, "Hallo receiver1");
        TIME_LOOP(1 * SECONDS, 1 * SECONDS) {
            msgToSend.cnt = counter;
	    msgToSend.stringMsg[14] = '#';
            PRINTF("at %9.3f sending %s %d\n", SECONDS_NOW(), msgToSend.stringMsg, msgToSend.cnt);

	    msgToSend.stringMsg[14] = '1';
            topic1.publish(msgToSend);
	    msgToSend.stringMsg[14] = '2';
            topic2.publish(msgToSend);
            counter++;
        }
    }
} myPublisher;
