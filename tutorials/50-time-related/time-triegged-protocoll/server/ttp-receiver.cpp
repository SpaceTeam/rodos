#include "rodos.h"
#include "../topics.h"
#include "mask.h"
#include "print.h"

/**
 * Receiver in the TTP Network.
 */
class TTPReceiver : public Subscriber {
  public:
    TTPReceiver() : Subscriber(ttpMessage) {}

    /**
     * Called when a node in the network sends during its time slot.
     */
    long put([[gnu::unused]] long int _topicId, [[gnu::unused]] long int _len, void* _msg, [[gnu::unused]] const NetMsgInfo& _netMsgInfo) {

        int64_t    arrivalTime = sysTime.getUTC();
        TTPMessage msg         = *(TTPMessage*)_msg;
        int32_t    nodeId      = msg.nodeId;

        PRINTF("%s %3.3f %s %d %s %d", arrivals[nodeId], toSeconds(arrivalTime), nodes[nodeId], static_cast<int>(msg.nodeId),
               messages[nodeId], static_cast<int>(msg.message));

        return 1;
    }
} ttpReceiver;
