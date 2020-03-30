#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 16", 1116);

struct JustPrint16 : public Subscriber {
    JustPrint16() : Subscriber(counter6, "justprint16") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R16, *(long*)msg);
        return 1;
    }
} justPrint16;

