#include "rodos.h"

struct SharedData {
    int  versionNr;
};


Topic<SharedData> deliverShared(-1, "deliverData");
Topic<SharedData> collectShared(-1, "CollectData");


/************************* Basis ***********************************/

struct BlackBoard {
    Semaphore protector;
    SharedData sharedData;
} blackBoard;

#define LK  blackBoard.protector.enter()
#define ULK blackBoard.protector.leave()
void deliverSharedCaller(SharedData &source)      { LK; blackBoard.sharedData = source;             ULK;}
void collectSharedCaller(SharedData &destination) { LK; destination = blackBoard.sharedData;        ULK;}
#undef LK
#undef ULK

SubscriberReceiver<SharedData> naneNotImportant01(deliverShared, deliverSharedCaller);
SubscriberReceiver<SharedData> naneNotImportant02(collectShared, collectSharedCaller);


/**********************************************************/

class Sender : public Thread {
public:
    SharedData myData;
    Sender() : Thread("Sender") { }
    void run () {
        myData.versionNr = 0;
        for (int i = 0; i < 10; i++) {
            PRINTF("Sending %d\n", myData.versionNr);
            deliverShared.publish(myData);
            myData.versionNr++;
            AT(NOW() + 0.1 * SECONDS);
        }
        
        AT(2 * SECONDS);
        hwResetAndReboot();
    }
} sender;

class Receiver : public Thread {
public:
    SharedData myData;
    Receiver() : Thread("Receiver") { }
    void run () {
        AT(NOW() + 0.05 * SECONDS);
        for (int i = 0; i < 5; i++) {
            collectShared.requestLocal(myData);
            PRINTF("    got %d\n", myData.versionNr);
            AT(NOW() + 0.3 * SECONDS);
        }
    }
} receiver;


