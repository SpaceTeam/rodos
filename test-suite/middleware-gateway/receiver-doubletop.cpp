#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

struct PrintDoubletop : public Subscriber {
    PrintDoubletop() : Subscriber(doubltop, "doubltop-printer") { }
    long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
        printfMask = 1;
        double  val = *(double*)msg;
        PRINTF("doubltop: %4.2f\n", val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printDoubletop;


