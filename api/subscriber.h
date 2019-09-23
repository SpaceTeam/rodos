
/**
* @file subscriber.h
* @date 2008/09/01 7:07
* @author Sergio Montenegro
*
*
* @brief subscribers for middleware
*
*/

#pragma once

#include "semaphore.h"
#include "topic.h"


namespace RODOS {

class Putter;

/**
* @class Subscriber
* @brief Subscriber to receive messages
*
*  The middleware communication is based on the publisher/subscriber protocol.
*  Publishers make messages public under a given topic.
*  Subscribers to a given topic get all messages which are published
*  under the given topic.
*  To establish a transfer path, both:
*  publisher and subscriber have to share the same topic.
*
* Each subscriber has a reference to the associated topic and a putter
* to store messages.
* All subscribers are registered in a list.
* Each time a messages is published the list of all subscriber will be search and
* for each subscriber where the topic matches the associated putter will be called
* to store a copy of the published message.
*/

class NetMsgInfo {
public:
    int32_t senderNode;	///< Node ID of sending instance of RODOS
    int64_t sentTime;   ///< Time in localTime units 
    uint32_t senderThreadId;
    uint32_t linkId;
};


class Subscriber : public ListElement {

    friend class TopicInterface;
    friend class TopicReporter;
    friend void initSystem();

public:
    /// Reference to a topic to which the subscriber listens.
    TopicInterface& topicInterface;

    /// Reference to the associated handler of a message received.
    Putter* receiver;

protected:
    /// Flag to indicate if the subscriber broadcasts a message internally or externally
    bool isAGateway;
    /// Flag to activate or deactivate the subscriber
    bool isEnabled;
    /// List of all subscribers in the system
    static List subscriberList;

    Semaphore protector;
    // DEPRECATED! DO not use anymore!
    //virtual long put(const long topicId, const long len, const void* data, long linkId);

    /// Default function: forward the message and invoke the receiver (putter). It locks the semaphore protector
    virtual long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo);

    /// do not lock any semaphore. Do not call any thread function
    /// default function resumes the associated thread (if defined) if it is waiting for it
    virtual void putFromInterrupt(const long topicId, const void* any, int len = 0);


public:

    /** At creation time a connection to the corresponding topic
     * and putter (receiver) has to be done.
     * Subscriber my be "Normal" message receiver or may be Gateways.
     * Gateways are interfaces to other memory spaces and nodes, e.g.
     * going through a network. Gateways just forward messages to other nodes.
     *
     * The parameter name is only used for debug purposes, to see which subscriber is
     * attached to which topic. Using names you can recognise present subscribers in listings.
     *
     * @param topic reference to the topic to which the owner will be subscribed
     * @param putter reference to a putter receiving messages from the topic subscribed
     * @param name name is used only for debug purposes, sing it one can recognise Threads.
     * @param isAGatewayParam : gateways get from all topics and they decide to forward or not
     */
    Subscriber(TopicInterface &topic, Putter& receiver, const char* name = "anonymSubscriber", bool isAGatewayParam=false);

    /**
      * Simple subscriber has no associated receiver (nor thread nor putter), they just (have to)
      * redefine the put method
      */
    Subscriber(TopicInterface &topic, const char* name = "anonymThreadSubscriber");

    /// Destructor, should not be called in nominal operation.
    virtual ~Subscriber() { RODOS_ERROR("Subscriber deleted"); }

    /** returns true if InputPort is enabled and wants to receive messages for
    *   given topicId
    */
    // virtual bool wants(const long topicId) const;

    /**
     * @return enabled == true -> receive all messages to the given ServiceId,
    *    else ignore them
    */
    void enable(bool onOff = true);

    /** Check whether the current subscriber is a gateway,
     * hence connects to the network.
     * @return if true -> message is sent to the network, false -> is send locally
     */
    bool isGateway() const;

};


/** Simple Subscriber interface for users ************/

template <class Type>
class SubscriberReceiver : public Subscriber {
    void (*receiverFunc)(Type &msg); // a function to be called for each message (optional)

public:
    SubscriberReceiver(TopicInterface &topic, const char* name = "anonymSubscriber") :
        Subscriber(topic, name) {
        receiverFunc = 0;
    }
    SubscriberReceiver(TopicInterface &topic,  void (*funcPtr)(Type&), const char* name = "anonymSubscriber") :
        Subscriber(topic, name) {
        receiverFunc = funcPtr;
    }

    virtual void put(Type &msg) {if(receiverFunc) (*receiverFunc)(msg); };
    virtual void put(Type &msg, const NetMsgInfo& netMsgInfo) {put(msg);};
    long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) { 
        put(*(Type*)data,netMsgInfo);
        return 1;
    }
};


/** a filter which may modify the content of published messages
 ** WARNING: Please be very careful when using filters
 ** do not abuse it. It is intended for voters, Kalman filter, etc
 **/


class TopicFilter {
public:
    /// will  be called before all subscribers
    virtual void prolog (const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) { }
    /// will  be called after all subscribers
    virtual void epilog(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) { }
};



}  // namespace

