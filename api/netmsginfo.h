
/**
* @file subscriber.h
* @date 2008/09/01 7:07
* @author Sergio Montenegro
*      Update: ting, 2022/03/10
*
*/

#pragma once

#include <stdint.h>

#include "thread.h"
#include "misc-rodos-funcs.h" // For getNodeNr
#include "stream-bytesex.h"   // To serialize/deserialize
#include "string_pico.h"      // for memcpy
#include "checksumes.h"

/******** Identification of Messages ************/

namespace RODOS {

enum class NetMsgType  : uint16_t { // set but not used until now
    PUB_SUB_MSG    = 0,
    P2P_UNRELIABLE = 1,
    P2P_RELIABLE   = 2,  // not used until now
    P2P_REGISTER_RECEIVER = 3,
    // P2P_ACK,  // not used until now
    REQUEST,
    RESPONSE,
    // TIME_SYNC, // not used until now
    // TOPIC_LIST, // not used until now but shall be the next
    // BROADCAST,  // not used until now
    // RECONFIG,  // not used until now
    // HEARTBEAT,  // not used until now
    // ERROR,  // not used until now
};


constexpr uint32_t RODOS_LOCAL_BROADCAST = 0;
constexpr uint32_t TOPIC_ID_FOR_TOPIC_REPORT = 0;


/**
 * Data structure containing information about topic messages sent via network (gateway).
 * Used only intern in the node. not transmited to the network,
 * See NetworkMessage, Why not the same? This is due to historical reasons
 * WARNING: Keep consistent with NetworkMessage !!!!
 */

class NetMsgInfo {
  public:
    int32_t    senderNode;     ///< Set by sendig application, set in publish()
    uint32_t   senderThreadId; ///< The ID of the sending thread, Set in publish()
    int64_t    sentTime;       ///< Time in localTime units, Set in pulbish()
    int32_t    receiverNode;   ///< Used only for network which do not support broadcast, and do not know about topics. -1 for broadcast
    uint32_t   linkId;         ///< The ID of the Linkinterface from which the message was received. Set by Linkinterface
    NetMsgType messageType;    ///< The type of the message, set by sender

    NetMsgInfo (NetMsgType type = NetMsgType::PUB_SUB_MSG) { init(type); }
    
    void init(NetMsgType type = NetMsgType::PUB_SUB_MSG) {
         linkId         = RODOS_LOCAL_BROADCAST;
         sentTime       = NOW();
         senderNode     = getNodeNumber();
         intptr_t ptr   = (intptr_t)(Thread::getCurrentThread());
         senderThreadId = static_cast<uint32_t>(ptr);
         messageType    = type;
         receiverNode   = -1; // Not used until now, but 0xffffffff shall be broadcast
    }
};

/**
 * Simple message data protocol to transmit data to a remote node.
 * Header is serialized NetMsgInfo, followed by the user data
 */
class NetworkMessage {
    static constexpr uint16_t HEADER_SIZE = 32;
    uint8_t header [HEADER_SIZE];
public:
    inline void    put_checkSum(uint16_t x)          {uint16_tToBigEndian(header+ 0, x); }
    inline void    put_senderNode(int32_t x)         {int32_tToBigEndian(header + 2, x); }
    inline void    put_sentTime (int64_t x)          {int64_tToBigEndian(header + 6, x); }
    inline void    put_senderThreadId(uint32_t x)    {uint32_tToBigEndian(header+14, x); }
    inline void    put_topicId(uint32_t x)           {uint32_tToBigEndian(header+18, x); }
    inline void    put_maxStepsToForward(int16_t x)  {int16_tToBigEndian(header +22, x); }
    inline void    put_len(uint16_t x)               {uint16_tToBigEndian(header+24, x); } // len of only user data
    inline void    put_type(uint16_t x)              {uint16_tToBigEndian(header+26, x); }
    inline void    put_receiverNode(int32_t x)       {int32_tToBigEndian(header +28, x); }

    inline uint16_t get_checksum()             const { return bigEndianToUint16_t(header+ 0); }
    inline int32_t  get_senderNode()           const { return bigEndianToInt32_t(header + 2); }
    inline int64_t  get_sentTime ()            const { return bigEndianToInt64_t(header + 6); }
    inline uint32_t get_senderThreadId()       const { return bigEndianToUint32_t(header+14); }
    inline uint32_t get_topicId()              const { return bigEndianToUint32_t(header+18); }
    inline int16_t  get_maxStepsToForward()    const { return bigEndianToInt16_t(header +22); }
    inline uint16_t get_len()                  const { return bigEndianToUint16_t(header+24); } // only user data
    inline uint16_t get_type()                 const { return bigEndianToUint16_t(header+26); }
    inline int32_t  get_receiverNode()         const { return bigEndianToInt32_t(header +28); }

    uint8_t userDataC[MAX_NETWORK_MESSAGE_LENGTH]; ///< local buffer for user data ca 1300. See platform-parameter.h

    /**
     * Copy constructor and assign operator that only copes nesacary data.
     */
    NetworkMessage() { }
    NetworkMessage(const NetworkMessage &oldMsg) { memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend()); }
    NetworkMessage& operator= (const NetworkMessage& oldMsg) {
        if(this != & oldMsg) memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend());
        return *this;
    }

    inline void dec_maxStepsToForward()    { put_maxStepsToForward(static_cast<int16_t>(get_maxStepsToForward() - 1)); }

    /** Copies user generated data into the message body.
     *
     * @param data pointer to input data stream to be transfered
     * @param length of message, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
     * @return length of message written
     */
    uint16_t setUserData(const void* data, uint16_t len);

    /** Copies data from message received to a user provided buffer.
     *
     * @param destination pointer to output data stream received
     * @param length of message buffer, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
     * @return length of message copied
     */
    uint16_t getUserData(void* destination, uint16_t maxLen);

    /* WARNING: Len has to be set befor you call this.  **/
    uint16_t numberOfBytesToSend() const { return static_cast<uint16_t>(HEADER_SIZE + get_len()); }
    uint16_t calculateCheckSum()         { return checkSum(header+2, HEADER_SIZE-2u + get_len()); } // header[0] and [1] hold the checksume selfs
    bool     isCheckSumOk()              { return calculateCheckSum() == get_checksum(); }
    void     setCheckSum()               { put_checkSum(calculateCheckSum()); }

} __attribute__((packed));

}  // namespace

