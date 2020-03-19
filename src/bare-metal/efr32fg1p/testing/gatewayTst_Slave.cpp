#include "rodos.h"
#include "gateway.h"
#include "hal/hal_radio_mstrslv.h"
#include "gateway/linkinterfaceradio.h"
#include "topic.h"

Topic<int> myTopic(1337, "MyTopic");

static HAL_RADIO_SLAVE slave; 
static LinkinterfaceRADIO linkinterfaceRADIO(NULL, &slave, false, -5001);
static Gateway gateway1(&linkinterfaceRADIO, true);

HAL_GPIO led(GPIO_024);

int id = 4;

static Application senderName("Publisher 01 simple", 1100);

class SlaveTst : public Thread {
	public:
	SlaveTst() : Thread("SlaveTst", 100, 1000) {}

	void run()
	{
		slave.config(PACKET_LENGTH, 300);
		slave.config(POLL_CHANNEL, 2);
		slave.config(DATA_CHANNEL, 3);
		slave.init();

		slave.config(SLAVE_ID, id);	// default Id = 0
		slave.executeState();

		TIME_LOOP(TIME_START_TX * MICROSECONDS, 100 * MICROSECONDS) 
		{
			if(slave.switchState() == 1) slave.executeState();
		}
	}
} slaveTst;

class MyPublisher01 : public Thread {
 
 public:
 	MyPublisher01() : Thread("SenderSimple", 100, 1000) { }

 	void run () {

		int value = 200;

 		TIME_LOOP(3*SECONDS, 500*MILLISECONDS) {
			myTopic.publish(value);
			value++;
		}
 	}
 } publishier01;

static class Update : public Thread {
public:
	Update() : Thread("Update", 100, 1000) {}

	void run()
	{
		int led_On = 0;

		led.config(GPIO_CFG_OUTPUT_ENABLE, 1);

		TIME_LOOP(5 * SECONDS, 10 * SECONDS)
		{
			PRINTF("Time: %.1f   ", SECONDS_NOW());
			PRINTF("Rx Counter: %d   Tx Counter: %d\n", slave.rxTimestampCounter, slave.txTimestampCounter);
			led.setPins(led_On);
			led_On = ~led_On;
		}
	}
} update;
