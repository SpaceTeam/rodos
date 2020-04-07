/**
* @file networkMessages.cpp
* @date 2014/09/30
* @author Sergio Montenegro
*
* @brief Messages to be exchanged between nodes and network
*
*/
#include <stdint.h>


#include "gateway/networkmessage.h"
#include "macros_bits_bytes.h"

namespace RODOS {

size_t NetworkMessage::setUserData(const void* data, size_t len) {
    len =  MIN(len, MAX_NETWORK_MESSAGE_LENGTH);
    memcpy(userDataC, data, len);
    put_len(static_cast<uint16_t>(len));
    return len;
}

/** Copies data from message received to a user provided buffer.
 *
 * @param destination pointer to output data stream received
 * @param length of message buffer, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
 * @return length of message copied
 */
size_t NetworkMessage::getUserData(void* destination, size_t maxLen) {
    maxLen = MIN(maxLen, get_len());
    memcpy(destination, userDataC, maxLen);
    return maxLen;
}

}
