
#pragma once

namespace RODOS {


/**
	Definition of Topic IDs for predefined RODOS services
	and user services

	services in RODOS: Topic Ids
	0    ...   99: RODOS internal
	100  ...  999: Input Output Services
	1000 ... 64K:  User Services: User defined Topic Ids
*/


/************ 0 ... 99: Rodos intern topics **************/

#define TOPIC_ID_SUBSCRIBERS_REPORT 	0u
#define TOPIC_ID_DEF_GATEWAY        	0u
#define TOPIC_ID_NETWORK_REPORT	 	1u

#define TOPIC_ID_TAKS_DISTRIBUTION	2u
#define TOPIC_ID_MONITORING_MSG		3u
#define TOPIC_ID_DEBUG_CMD_MSG		4u


/************ 100 ... 999:  Input / Output services ***/

#define TOPICID_UART_CONF 		110u
#define TOPICID_UART_IN			111u
#define TOPICID_UART_OUT		112u

#define TOPIC_ID_PWM_CONF		120u
#define TOPIC_ID_PWM_WIDTH		121u

#define TOPIC_ID_ANALOG_CONF		123u
#define TOPIC_ID_ANALOG_CHANS		124u

/******************************************************/

#define FIRST_USER_TOPIC_ID 		1000u

}  // namespace


