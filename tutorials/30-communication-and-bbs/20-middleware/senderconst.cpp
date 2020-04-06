#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 01 const", 1100);

class MyPublisherC : public StaticThread<> {
public:
	MyPublisherC() : StaticThread<>("SenderConst") { }
	void run () {
		TIME_LOOP(3*SECONDS, 3*SECONDS) {
			PRINTF(CONST "%lld\n", NOW());
			counter2.publish(100);
		}
	}
} publisherC;


