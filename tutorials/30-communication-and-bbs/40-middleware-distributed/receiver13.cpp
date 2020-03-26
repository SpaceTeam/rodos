#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 13", 1113);

struct JustPrint13 : public Subscriber {
    JustPrint13() : Subscriber(counter3, "justprint13") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R13, *(long*)msg);
        return true;
    }
} justPrint13;

