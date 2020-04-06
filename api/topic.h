

/**
 * @file topic.h
 * @date 2008/09/01 7:07
 * @author Sergio Montenegro
 *
 *
 * @brief topic for middleware
 *
 */

#pragma once

#include "listelement.h"
#include "rodos-debug.h"
#include "gateway/networkmessage.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

class NetMsgInfo;
class TopicFilter;

#define	RODOS_LOCAL_BROADCAST	0

/**
 *  @class TopicInterface
 *  @brief TopicInterface only for internal use
 *
 *  The middleware communication is based on the publisher/subscriber protocol.
 *  Publishers make messages public under a given topic.
 *  Subscriber to a given topic get all messages which are published
 *  under the given topic.
 * To establish a transfer path, both,
 * publisher and subscriber have to share the same topic.
 *
 * A Topic is a pair: data-type and an integer representing
 * a topic identifier.
 *
 * To access topics in an generic way which is independent of
 * data type and topicId, a TopicInterface is created.
 *
 */

class TopicInterface : public ListElement {

	friend void initSystem();
	friend class Gateway;
	friend class Subscriber;
	friend class TopicReporter;

public:


//protected:
	static List topicList; ///< List of all topics present in the system
	List mySubscribers; ///< List of pointers to subscribers associated to one topic instance
        TopicFilter* topicFilter; ///< a filter may modify the content of the message befor the subscriver get it.
        
	long topicId;   ///< Topic ID used for identification by network tramsmitions
	long msgLen;    ///< Size of message transfered via this topic
	bool onlyLocal; ///< if true, never call the gateways for this topic, even if publish says ditritribute to network



public:

    TopicInterface(long id, long len, const char* name, bool _onlyLocal = false);

    virtual ~TopicInterface() { 
        if(isShuttingDown) return;
        RODOS_ERROR("Topic Deleted");
    }

    /** publish/distribute the message to all listeners to the given serviceId.
     * if shallSendToNetwork the message will be forwarded to gateways too (default).
     * warning 1: Never use it from an interrupt server.
     * warning 2: the pointer to msg will be distributed. A Subscriber may modify its content
     */
    unsigned long publish(void *msg, bool shallSendToNetwork = true,
    		NetMsgInfo* netMsgInfo = 0);

    /** Usually we use the length of the corresponding data type associated to the topic
     * but for some exceptional cases, like for strings, we may send shorter messages.
     * warning 1: Never use it from an interrupt server.
     * warning 2: the pointer to msg will be distributed. A Subscriber may modify its content
     */
    unsigned long publishMsgPart(void *msg, unsigned int lenToSend,
            bool shallSendToNetwork = true, NetMsgInfo* netMsgInfo = 0);

    /** Publishfrom interrupts uses no semaphores as protection!
      * the Subscriber shall use no thread operations
      * the Subscriber shall be as short as possible
      */
    void publishFromInterrupt(void *any, int len = 0);

    /** request is like publish, but the calles expects data back instead of sending it.
      * The internal mechanims is identical the same like publisch, the middleware will
      * call puuters from subscribers. Such puters shall proivde data instad of geting it.
      **/

     inline unsigned long requestLocal(void *msg) { return publish(msg, false); }

     /// return 0 it not found
     static TopicInterface* findTopicId(long wantedTopicId);

     void setTopicFilter(TopicFilter* filter);

};


/**
 *  @class Topic
 *  @brief Topic  to patch publisher and subscribers
 *
 *  The middleware communication is based on the publisher/subscriber protocol.
 *  Publishers make messages public under a given topic.
 *  Subscriber to a given topic get all messages which are published
 *  under the given topic.
 * To establish a transfer path, both,
 * publisher and subscriber have to share the same topic.
 *
 * A Topic consists of a pair: a data-type and an integer representing
 * the topic identifier.
 *
 * Topic provides methods to publish and distribute messages.
 *
 * The current implementation accepts only shorts as topicId
 * because of limitations on the FPGA (Hardware) Middleware
 * implementation.
 *
 * topic id 0 is reserved for middleware messages (eg. topic lists)
 * if you use topic id -1 the system will generate an id according
 * to the name used in the constructor. This number will be
 * compatible with all hardware implementations. We recommend
 * to use this feature. This is compatible with our hardware
 * networks.
 * If you prefer to use your own IDs, please use numbers bigger than 1000
 * see reserved_topic_ids.h
 *
 */

/*************************************/

template <class Type>
class Topic : public TopicInterface {
public:

    /** constructor, parameter name is used for debug purposes and as hash value
      * to generate a topic id if it was defined as -1. This is the proposed
       * method.
      */
    Topic(long id, const char* name, bool _onlyLocal = false) : TopicInterface(id, sizeof(Type), name, _onlyLocal) { }

    ~Topic() {
        if(isShuttingDown) return;
        RODOS_ERROR("topic deleted");
    }

    /** publish/distribute the message to all listeners to the given serviceId.
     * if shallSendToNetwork the message will be fordarded to gateways too (default)
     * warning 1: Never use it from an interrupt server.
     * warning 2: the pointer to msg will be distributed. A Subscriber may modify its content
     */
    inline unsigned long publish(Type &msg, bool shallSendToNetwork = true) {
        return TopicInterface::publish(&msg, shallSendToNetwork);
    }

    inline unsigned long publish(Type &&msg, bool shallSendToNetwork = true) {
        return TopicInterface::publish(&msg, shallSendToNetwork);
    }

    /** To publish constants,
     * But please only for basic data types (char, short, long, float, double)
     */
    [[deprecated("Use publish(...) also for rvalues.")]]
    inline unsigned long publishConst(Type msgConst, bool shallSendToNetwork = true) {
        Type msg = msgConst;
        return TopicInterface::publish(&msg, shallSendToNetwork);
    }


    /** To send only a part of the data type, and not the whole object.
      *for example for strings
      * warning: Never us it from an interrupt server.
      */
    inline unsigned long publishMsgPart(Type &msg, unsigned int lenToSend, bool shallSendToNetwork = true) {
        return TopicInterface::publishMsgPart(&msg, lenToSend, shallSendToNetwork);
    }

    /** request is like publish, but the calles expects data back instead of sending it.
      * The internal mechanims is identical the same like publisch, the middleware will
      * call puuters from subscribers. Such puters shall proivde data instad of geting it.
      **/

     inline unsigned long requestLocal(Type &msg) { return TopicInterface::publish(&msg, false); }

};

/***
 ** very often we need a generiv type for topics which contians just
 ** a pointer and a len.
 ** to avoid 100x declarations fo this struct, we define ohe here
 **/

struct GenericMsgRef { // V. 128 PTS
    uint64_t  context; ///< any further info you would like to deliver
    char*     msgPtr;
    int32_t   msgLen; // Warning: It has to be signed, else endless loop at publish

    GenericMsgRef() { msgLen = context = 0; msgPtr = 0; }
};

/**************************************************************/
/** A topic only for gateways */

extern Topic<GenericMsgRef> defaultGatewayTopic; //All localy publiched Topics go here for the Gateways and/or Routers to send out
extern Topic<NetworkMessage> defaultRouterTopic; //All incoming Messages are publiched here by the Gateways.
/************ Predifined Topics from Interrupt servers **********/

extern Topic<void*> interruptTimer;
extern Topic<void*> interruptUart;
extern Topic<void*> interruptSigterm;
extern Topic<GenericMsgRef> charInput; ///< used instead of getcharNoWait()


}  // namespace

