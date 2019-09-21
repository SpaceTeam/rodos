#include "rodos.h"
#include "common.h"

namespace APP04 {

class AppT3: public Thread {
public:
    AppT3() : Thread("AppT04-3:", 400)  { }
    void run() {
        TIME_LOOP(1400*MILLISECONDS, 1*SECONDS) {
            PRINTF("    APP4-3:\n");
        } // loop
    }

} appT3;

} // namesapce
