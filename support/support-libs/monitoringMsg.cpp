
#include "rodos.h"
#include "monitoringMsg.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


MessageTypeId EOM(int id) {
	MessageTypeId mti;
	mti.id = id;
	return mti;
}

/*****************/
void MonitoringMsg::clear() {
    numOfParamsToSend = 0;
    numOfParamsDeserialized = 0;
    msgType = 0;
}

MonitoringMsg::MonitoringMsg(uint8_t taskId) {
    clear();
    nodeNr = static_cast<uint8_t>(getNodeNumber());
	this->taskId = taskId;
}


bool MonitoringMsg::serialize(void* src, size_t len) {
    uint8_t newEnd = numOfParamsToSend + static_cast<uint8_t>(len);
    if (newEnd >= MAX_PARAMS) return false;
    memcpy(&params[numOfParamsToSend], src, len);
    numOfParamsToSend = newEnd;
    return true;
}



void MonitoringMsg::report(int id) {
    sendTime = NOW();
    msgType = id;
    monitorMsgTopic.publishMsgPart(*this, numOfBytesToSend(), true);
    clear();
}

/**********************************/
/** For the monitor side   ********/
/**********************************/

bool MonitoringMsg::deserialize(void* dest, size_t len) {
    uint8_t newEnd = numOfParamsDeserialized + static_cast<uint8_t>(len);
    if (newEnd > numOfParamsToSend)  return false; 
    memcpy(dest, &params[numOfParamsDeserialized], len);
    numOfParamsDeserialized = newEnd;
    return true;
}

void* MonitoringMsg::deserializeNoCopy(size_t len) {
    char* currentPos =  &params[numOfParamsDeserialized];
    numOfParamsDeserialized += static_cast<uint8_t>(len);
    return currentPos;
}

Topic<MonitoringMsg> monitorMsgTopic(TOPIC_ID_MONITORING_MSG, "monitorMsgTopic");
Topic<MonitoringMsg>  dbgCommandTopic(TOPIC_ID_DEBUG_CMD_MSG,"dbgCommandTopic");


#ifndef NO_RODOS_NAMESPACE
}
#endif


