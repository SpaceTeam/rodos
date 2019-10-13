
#include "rodos.h"

/**************************************************** Common Data ***/

struct Greetings {
    int64_t date;
    char    msg[80];
};

Topic<Greetings> valentine(20, "valentine");


/********************************************************* Charly  ***/

class Charly : public TopicFilter {
  public:
    void prolog(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
        Greetings* grts = (Greetings*)data;
        grts->msg[0]    = 'I'; // correction
    }
    void epilog(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
        PRINTF("charly knows, message was distributed\n");
    }
} charly;


/************************************************************ Bob ***/

class Bob : public Thread {
    Greetings greetings;

  public:
    void run() {
        greetings.date = 111;
        strcpy(greetings.msg, "i wish you would be here");
        PRINTF("Bob sending greetings '%s'\n", greetings.msg);
        valentine.publish(greetings);

        valentine.setTopicFilter(&charly);
        valentine.publish(greetings);

        AT(500*MILLISECONDS);
        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();
    }
} bob;

/********************************************************* Alice  ***/

class Alice : public SubscriberReceiver<Greetings> {
  public:
    Alice() : SubscriberReceiver<Greetings>(valentine, "aliceSubs") {}
    void put(Greetings& data) {
        Greetings* greetingsForMe = (Greetings*)&data;
        PRINTF("Alice got date %lld:  msg: %s\n", greetingsForMe->date, greetingsForMe->msg);
    }
} alice;
