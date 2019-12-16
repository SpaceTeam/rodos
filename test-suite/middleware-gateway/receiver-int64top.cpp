#include "rodos.h"
#include "demo_topics.h"


struct PrintInt64Top : public Subscriber {
    PrintInt64Top() : Subscriber(int64top, "int64top-printer") { }
    long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
        int64_t val = *(int32_t*)msg;
        PRINTF("int64top: %8lld\n", val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printInt64Top;


