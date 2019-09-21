
#include "rodos.h"

/**
* @file subscriber.cc
* @date 2008/09/01 7:07
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief topic for middleware
*
*/

// List Subscriber::subscriberList = 0;



namespace RODOS {

Subscriber::Subscriber(TopicInterface& topic, Putter& receiver, const char* name, bool isAGatewayParam) :
    ListElement(subscriberList),  topicInterface(topic) {
    this->isAGateway = isAGatewayParam;
    this->isEnabled = true;
    this->name = name;
    this->receiver = &receiver;
}

Subscriber::Subscriber(TopicInterface& topic, const char* name) :
    ListElement(subscriberList),  topicInterface(topic) {
    this->isAGateway = false;
    this->isEnabled = true;
    this->name = name;
    this->receiver = 0;
}


//bool Subscriber::wants(const long topicId) const {
//	return (isEnabled && (topicId == topicInterface.topicId));
//}

void Subscriber::enable(bool onOff) { isEnabled = onOff; }

bool Subscriber::isGateway() const { return isAGateway; }


/**
 * Forward the message to the Subscriber owning receiver: a putter
 */


long Subscriber::put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
    if(!isEnabled) return 0;
    protector.enter();
    if(receiver) receiver->putGeneric(topicId, len,data, netMsgInfo);
    protector.leave();
    return receiver? 1 : 0;
}


void Subscriber::putFromInterrupt(const long topicId, const void* any, int len) {
    if(receiver) {
        NetMsgInfo dummy;
        receiver->putGeneric(topicId, len, any, dummy);
    }
}


}


