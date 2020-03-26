#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 14", 1114);

struct JustPrint14 : public Subscriber {
    JustPrint14() : Subscriber(counter4, "justprint14") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R14, *(long*)msg);
        return 1;
    }
} justPrint14;

