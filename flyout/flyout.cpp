#include "rodos.h"
#include "hal_gpio.h"

static Application module01("LED_Test", 2001);

HAL_GPIO ledGreen(GPIO_005);

class LedTest : public StaticThread<>
{
public:
    void init()
    {
        ledGreen.init(true);
    }

    void run()
    {
        auto toggle = 1U;
        ledGreen.setPins(toggle);
        TIME_LOOP(2 * SECONDS, 500 * MILLISECONDS)
        {
            toggle ^= 1U;
            ledGreen.setPins(toggle);
        }
    }
};

LedTest ledTest;
