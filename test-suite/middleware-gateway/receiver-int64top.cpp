#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

struct PrintInt64Top : public Subscriber {
    PrintInt64Top() : Subscriber(int64top, "int64top-printer") { }
    long put([[gnu::unused]] const long topicId, [[gnu::unused]] const long len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        printfMask = 1;
        int64_t val = *(int32_t*)msg;
        PRINTF("int64top: %8lld\n", val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printInt64Top;


