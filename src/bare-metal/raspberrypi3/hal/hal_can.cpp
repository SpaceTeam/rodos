
#include "hal/hal_can.h"

#include "rodos.h"

namespace RODOS {


HAL_CAN::HAL_CAN([[gnu::unused]] CAN_IDX canIdx, [[gnu::unused]] GPIO_PIN rxPin , [[gnu::unused]] GPIO_PIN txPin){

}

int HAL_CAN::init([[gnu::unused]] uint32_t baudrate) {
    return 0;
}

void HAL_CAN::reset(){
}

int HAL_CAN::config([[gnu::unused]] CAN_PARAMETER_TYPE type, [[gnu::unused]] int paramVal){
	return -1;
}

CanErrorMsg HAL_CAN::status([[gnu::unused]] CAN_STATUS_TYPE type){
  CanErrorMsg errorMsg;
  errorMsg.errorType = CAN_ERROR_TYPE::UNKNOWN;
  errorMsg.canErrorCnt = 0;
	return errorMsg;

}

bool  HAL_CAN::isWriteFinished(){
	return true;

}

bool  HAL_CAN::isDataReady(){
	return false;
}

bool  HAL_CAN::addIncomingFilter([[gnu::unused]] uint32_t ID, [[gnu::unused]] uint32_t IDMask, [[gnu::unused]] bool extID, [[gnu::unused]] bool rtr){
		return false;
}

int HAL_CAN::write([[gnu::unused]] const uint8_t* sendBuf, [[gnu::unused]] uint8_t len, [[gnu::unused]] uint32_t canID, [[gnu::unused]] bool extID, [[gnu::unused]] bool rtr){

	return 0;
}

int HAL_CAN::read([[gnu::unused]] uint8_t* recBuf, [[gnu::unused]] uint32_t* canID, [[gnu::unused]] bool* isExtID, [[gnu::unused]] bool* rtr){
	return -1;
}


} /* namespace RODOS */
