#include "rodos.h"
#include "gateway.h"
#include "hal/hal_radio_mstrslv.h"

static HAL_RADIO_MASTER master; 
static LinkinterfaceRADIO linkinterfaceRADIO(&master);
static Gateway gateway1(&linkinterfaceRADIO, true);

class MasterTst : public Thread {
	public:
	MasterTst() : Thread("MasterTst") {}

	void run()
	{
		master.executeState();

		TIME_LOOP(TIME_START_TX * MICROSECONDS, 100 * MICROSECONDS) 
		{
			if(master.switchState() == 1) master.executeState();
		}
	}
} masterTst;
