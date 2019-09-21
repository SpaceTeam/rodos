


/**
* @file networkMessages.h
* @date 2013/06/30
* @author Sergio Montenegro
*
* @brief Messages to be exchanged between nodes and network
*
*/

#pragma once

#include <stdint.h>
#include "platform-parameter.h"
#include "checksumes.h"
#include "string_pico.h"
#include "debug.h"
#include "stream-bytesex.h"

/******** Generic Network Message ***************/

namespace RODOS {

/**
 * Simple message data protocol to transmit data to a remote node.
 */
class NetworkMessage {
    static const int HEADER_SIZE = 26;
    uint8_t header [HEADER_SIZE];
public:
    inline void    put_checkSum(uint16_t x)          {int16_tToBigEndian(header+0, x); }
    inline void    put_senderNode(int32_t x)         {int32_tToBigEndian(header+2, x); }
    inline void    put_sentTime (int64_t x)          {int64_tToBigEndian(header+6, x); }
    inline void    put_senderThreadId(uint32_t x)    {int32_tToBigEndian(header+14, x); }
    inline void    put_topicId(uint32_t x)           {int32_tToBigEndian(header+18, x); }
    inline void    put_maxStepsToForward(int16_t x)  {int16_tToBigEndian(header+22, x); }
    inline void    put_len(uint16_t x)               {int16_tToBigEndian(header+24, x); }

    inline uint16_t get_checksum()             const { return bigEndianToInt16_t(header+0); }
    inline int32_t  get_senderNode()           const { return bigEndianToInt32_t(header+2); }
    inline int64_t  get_sentTime ()            const { return bigEndianToInt64_t(header+6); }
    inline uint32_t get_senderThreadId()       const { return bigEndianToInt32_t(header+14); }
    inline uint32_t get_topicId()              const { return bigEndianToInt32_t(header+18); }
    inline int16_t  get_maxStepsToForward()    const { return bigEndianToInt16_t(header+22); }
    inline uint16_t get_len()                  const { return bigEndianToInt16_t(header+24); }

    uint8_t userDataC[MAX_NETWORK_MESSAGE_LENGTH]; ///< local buffer for user data

    /**
     * Copy constructor and assign operator that only copes nesacary data.
     */
    NetworkMessage() { }
    NetworkMessage(const NetworkMessage &oldMsg) { memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend()); }
    NetworkMessage& operator= (const NetworkMessage& oldMsg) {
        if(this != & oldMsg) memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend());
        return *this;
    }

    inline void dec_maxStepsToForward()    { put_maxStepsToForward(get_maxStepsToForward() - 1); }

    /** Copies user generated data into the message body.
     *
     * @param data pointer to input data stream to be transfered
     * @param length of message, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
     * @return length of message written
     */
    int32_t setUserData(const void* data, uint32_t len);

    /** Copies data from message received to a user provided buffer.
     *
     * @param destination pointer to output data stream received
     * @param length of message buffer, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
     * @return length of message copied
     */
    uint32_t getUserData(void* destination, uint32_t maxLen);

    /* WARNING: Len has to be set befor you call this.  **/
    uint32_t numberOfBytesToSend() const { return HEADER_SIZE + get_len(); }
    uint16_t calculateCheckSum()         { return checkSum(header+2, HEADER_SIZE-2 + get_len()); }
    bool     isCheckSumOk()              { return calculateCheckSum() == get_checksum(); }
    void     setCheckSum()               { put_checkSum(calculateCheckSum()); }

} __attribute__((packed));


/***** Topic list reports for network management ***/

class TopicListReport {
public:
    int32_t numberOfTopics;             ///< total number of topics assigned for the network
    int16_t topicList[MAX_SUBSCRIBERS]; ///< simple list of topic IDs subscribed for network

    TopicListReport() { init(); }      ///< Constructor, initializes topicList.
    void init() { numberOfTopics = 0; }

    /** Look up list, whether a topic is set for network messages.
     * @param[in] topicId ID to check whether topic is listed
     */
    bool find(const short topicId) const {
        for (int i = 0; i < numberOfTopics; i++) {
            if (topicId == topicList[i]) {
                return true;
            }
        }
        return false;
    }

    /** Adds a new topic to list, if not already present.
     * @param[in] topicId ID to add to the list
     */
    void add(const short topicId) {
        if (numberOfTopics < MAX_SUBSCRIBERS - 1) {
            if (find(topicId)) {
                return;
            }
            topicList[numberOfTopics] = topicId;
            numberOfTopics++;
        } else {
            ERROR("No more topics available\n");
        }
    }

    /// returns the size needed for transmission
    int numberOfBytesToSend() { return (sizeof(long) + numberOfTopics * sizeof(topicList[0])); }
};


}  // namespace



