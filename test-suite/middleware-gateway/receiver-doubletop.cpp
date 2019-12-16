#include "rodos.h"
#include "demo_topics.h"


struct PrintDoubletop : public Subscriber {
    PrintDoubletop() : Subscriber(doubltop, "doubltop-printer") { }
    long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
        double  val = *(double*)msg;
        PRINTF("doubltop: %4.2f\n", val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printDoubletop;


