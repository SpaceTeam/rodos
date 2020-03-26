#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 11", 1111);

struct JustPrint11 : public Subscriber {
    JustPrint11() : Subscriber(counter1, "justprint11") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld",  R11, *(long*)msg);
        PRINTF("%s%lld", MSGCNT, getNumberOfReceivedMsgsFromNetwork());
        return true;
    }
} justPrint11;

