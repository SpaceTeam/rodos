#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

struct PrintInt32Top : public Subscriber {
    PrintInt32Top() : Subscriber(int32top, "int32top-printer") { }
    long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
        printfMask = 1;
        int32_t val = *(int32_t*)msg;
        PRINTF("int32top: %4d\n",  val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printInt32Top;


