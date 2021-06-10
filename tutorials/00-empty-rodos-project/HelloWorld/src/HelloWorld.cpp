#include "rodos.h"

// STM32F4DISCOVERY LED Pins
#ifdef USE_STM32_DISCOVERY
GPIO_PIN GREEN  = GPIO_060;
GPIO_PIN ORANGE = GPIO_061;
GPIO_PIN RED    = GPIO_062;
GPIO_PIN BLUE   = GPIO_063;

HAL_GPIO green(GREEN);
HAL_GPIO orange(ORANGE);
HAL_GPIO red(RED);
HAL_GPIO blue(BLUE);

class HelloWorld : StaticThread<> {
public:
	HelloWorld(const char* name) : StaticThread(name) { }

private:
	void init() {
		green.init(true, 1, 0);
		orange.init(true, 1, 0);
		red.init(true, 1, 0);
		blue.init(true, 1, 0);
	}

	void run() {
		uint32_t pinVal = 1;
		TIME_LOOP(0, 1 * SECONDS) {
			PRINTF("Hello world\n");
			green.setPins(pinVal);
			orange.setPins(pinVal);
			red.setPins(pinVal);
			blue.setPins(pinVal);

			pinVal = pinVal == 1 ? 0 : 1;
		}
	}
};

#else
class HelloWorld : StaticThread<> {
public:
	HelloWorld(const char* name) : StaticThread(name) { }

private:
	void init() { }

	void run() {
		TIME_LOOP(0, 1 * SECONDS) {
			PRINTF("Hello World\n");
		}
	}
};
#endif /* USE_STM32_DISCOVERY */

static HelloWorld helloWorld("HelloWorld");
