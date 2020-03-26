#include "rodos.h"

/*
 * In this test, one thread requests data from a subscriber.
 */

Topic<char[20]>   	collectData(-1, "CollectData");

/******************************/

class Collector : public StaticThread<> {
public:
    Collector() : StaticThread<>("Collector") { }

    void run () {
        char buf[20];
        for (int i = 0; i < 10; i++) {
            PRINTF("Colleting data\n");
            collectData.requestLocal(buf);
            printCharsHex("Collected:", buf, 20);
            AT(0.1*SECONDS);
        }
        
        AT(500*MILLISECONDS);
        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();
    }
} collector;


class FillData : public Subscriber {
public:
    FillData() :  Subscriber(collectData, "fillData") { }
        long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
		char* toWrite = (char*)msg;
		static int cnt = 0;
		for(int i = 0; i < 20; i++) toWrite[i] = cnt*0x10 + i;
		cnt++;
		return 1;
	}
} fillData;

