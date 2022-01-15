#include "rodos.h"

#ifdef STM32F40_41xxx
constexpr char platformName[] = "STM32F407";
#endif /*STM32F40_41xxx */
#ifdef STM32F411xE
constexpr char platformName[] = "STM32F411";
#endif /* STM32F411xE */
#if !defined(STM32F40_41xxx) && !defined(STM32F411xE)
constexpr char platformName[] = "linux-x86";
#endif /* !defined(STM32F40_41xxx) && !defined(STM32F411xE) */

// STM32F4DISCOVERY LED Pins
#ifdef USE_STM32_DISCOVERY
// Using Discovery Board LEDs
GPIO_PIN GREEN  = GPIO_060;
GPIO_PIN ORANGE = GPIO_061;
GPIO_PIN RED    = GPIO_062;
GPIO_PIN BLUE   = GPIO_063;

HAL_GPIO green(GREEN);
HAL_GPIO orange(ORANGE);
HAL_GPIO red(RED);
HAL_GPIO blue(BLUE);
#endif /* USE_STM32_DISCOVERY */

class MyFirstThread : StaticThread<> {
public:
	MyFirstThread(const char* name) : StaticThread(name) { }

private:
	void init() {
#ifdef USE_STM32_DISCOVERY
		green.init(true, 1, 0);
		orange.init(true, 1, 0);
		red.init(true, 1, 0);
		blue.init(true, 1, 0);
#endif /* USE_STM32_DISCOVERY */
	}

	void run() {
		PRINTF("Platform: %s\n", platformName);
		uint32_t pinVal = 1;
		TIME_LOOP(2 * SECONDS, 1 * SECONDS) {
			PRINTF("%s says \"Hello world!\"\n", getName());
#ifdef USE_STM32_DISCOVERY
			green.setPins(pinVal);
			orange.setPins(pinVal);
			red.setPins(pinVal);
			blue.setPins(pinVal);
#endif /* USE_STM32_DISCOVERY */
			pinVal ^= 1;
		}
	}
};

static MyFirstThread helloWorld("MyFirstThread");
