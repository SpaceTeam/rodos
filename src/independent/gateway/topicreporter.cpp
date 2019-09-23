/*
 * topicreporter.cpp
 *
 *  Created on: 28.03.2013
 *      Author: Dilger
 */

#include "rodos.h"

#include "gateway/topicreporter.h"

namespace RODOS {

#define not0(a) ((a)?1:0)

TopicReporter::TopicReporter(Gateway* gateway1, Gateway* gateway2, Gateway* gateway3, Gateway* gateway4) {
    gateways[0]      = gateway1;
    gateways[1]      = gateway2;
    gateways[2]      = gateway3;
    gateways[3]      = gateway4;
    numberOfGateways = not0(gateway1) + not0(gateway2) + not0(gateway3) + not0(gateway4);
}

void TopicReporter::run() {
    TIME_LOOP(1700*MILLISECONDS, 1*SECONDS) {
        topiclist.init();
        sendListOfTopicsToNetwork();
    }
}

void TopicReporter::sendListOfTopicsToNetwork() {
    addLocalTopics(topiclist);
    sendToAllGateways(topiclist);
        //PRINTF("Topicreporter (%d):",topiclist.numberOfTopics);
        //for(int i = 0; i < topiclist.numberOfTopics; i++) PRINTF(" %d ", topiclist.topicList[i]);
        //PRINTF("\n");
}


void TopicReporter::addLocalTopics(TopicListReport& list) {
    ITERATE_LIST(TopicInterface, TopicInterface::topicList) { // scan all subscriver for each topic
        for(Subscriber* subs = (Subscriber*)iter->mySubscribers; subs!=0; subs = (Subscriber*)subs->getNext()) {
            if(subs->isEnabled) { // at least one listener
                list.add((short)(iter->topicId));
                break; // this topic is processed, do not search mor subscriver for thie topic
            }
        }  
    }
}


void TopicReporter::addTopicsFromGateway(TopicListReport& list, Gateway* gateway) {
    TopicListReport* gatewayTopics = gateway->getTopicsToForward();

    for(int i=0; i<gatewayTopics->numberOfTopics; i++) {
        list.add(gatewayTopics->topicList[i]);
    }

}


void TopicReporter::sendToAllGateways(TopicListReport& list) {
    prepareNetworkMessage(topicMessage,0,&list,list.numberOfBytesToSend());
    for(int i=0; i<numberOfGateways; i++) {
        gateways[i]->sendNetworkMessage(topicMessage);
    }
}


void TopicReporter::sendToGateway(TopicListReport& list, Gateway* gateway) {
    prepareNetworkMessage(topicMessage,0,&list,list.numberOfBytesToSend());
    gateway->sendNetworkMessage(topicMessage);
}


void TopicReporter::addGateway(Gateway* gateway) {
    gateways[numberOfGateways]= gateway;
    numberOfGateways++;

    if(numberOfGateways > MAX_NUMBER_OF_GATEWAYS_PER_ROUTER) {
        RODOS_ERROR("Too many Gateway added to a TopicReporter\n");
        numberOfGateways--;
    }
}

}
