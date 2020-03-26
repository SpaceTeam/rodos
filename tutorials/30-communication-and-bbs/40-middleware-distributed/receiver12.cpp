#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 12", 1112);

struct JustPrint12 : public Subscriber {
    JustPrint12() : Subscriber(counter2, "justprint12") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R12, *(long*)msg);
        return 1;
    }
} justPrint12;
