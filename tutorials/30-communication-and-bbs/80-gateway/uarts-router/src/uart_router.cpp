#include <stddef.h>

#include "uart_router.h"
#include "gateway/gateway.h"
#include "timemodel.h"
#include "netmsginfo.h"

namespace RODOS {

// Gateway lookup table:
// > bit number of "receiverNodesBitMap" is the index
// > content is the assigned Gateway (=> Linkinterface => UART-Port)
static Gateway *g_gateway_lookup[MAX_NUM_OF_NODES] = {nullptr};

void UartRouter::routeMsg(NetworkMessage &msg, uint32_t linkid) {
    uint32_t sender_bitnr, receiver_bit, bitmap;
    sender_bitnr = static_cast<uint32_t>(msg.get_senderNode()) % MAX_NUM_OF_NODES;
    bitmap       = msg.get_receiverNodesBitMap();

    // I've never seen messages from this sender...
    if (g_gateway_lookup[sender_bitnr] == nullptr) {
        //...therefore I need to search for the associated Gateway...
        for (size_t j = 0; j < this->numberOfGateways; j++) {
            if (this->gateways[j]->getLinkIdentifier() == linkid) {
                //...and remember it
                g_gateway_lookup[sender_bitnr] = this->gateways[j];
                break;
            }
        }
    }

    if (!shouldRouteThisMsg(msg, linkid)) return;

    receiver_bit = 1u;
    for (size_t i = 0; i < MAX_NUM_OF_NODES; i++, receiver_bit <<= 1) {
        if ((bitmap & receiver_bit) == 0 || g_gateway_lookup[i] == nullptr)
            continue;

        if (shouldRouteThisMsgToGateway(msg, linkid, g_gateway_lookup[i])) {
            g_gateway_lookup[i]->sendNetworkMessage(msg);
        }
    }
}

} /* namespace RODOS */
