#include "rodos.h"
#include "hal/hal_radio_mstrslv.h"

HAL_RADIO_MASTER master;
char example_msg[MAX_PACKET_LENGTH];
HAL_GPIO led(GPIO_024);

class MasterTst : public Thread {
	public:
	MasterTst() : Thread("MasterTst") {}

	void run()
	{
		master.init();
		master.config(POLL_CHANNEL, 2);
		master.config(DATA_CHANNEL, 3);

		master.executeState();

		TIME_LOOP(TIME_START_TX * MICROSECONDS, 100 * MICROSECONDS) 
		{
			if(master.switchState() == 1) master.executeState();
		}
	}
} masterTst;


static class MessageTst : public Thread {
public:
	MessageTst() : Thread("MessageTst") {}

	void run()
	{
		TIME_LOOP(5 * MILLISECONDS, 25 * MILLISECONDS) 
		{
			if(master.read(example_msg) > 0)
			{
				master.write(example_msg, 1500);
			}
		}
	}

	void init() { PRINTF("Read&Write"); }

} messageTst;


static class Update : public Thread {
public:
	Update() : Thread("Update") {}

	void run()
	{
		int led_On = 0;

		led.config(GPIO_CFG_OUTPUT_ENABLE, 1);

		TIME_LOOP(5 * SECONDS, 10 * SECONDS)
		{
			PRINTF("Time: %.1f   ", SECONDS_NOW());
			PRINTF("Rx Counter: %d   Tx Counter: %d\n", master.rxTimestampCounter, master.txTimestampCounter);
			led.setPins(led_On);
			led_On = ~led_On;
		}
	}
} update;
