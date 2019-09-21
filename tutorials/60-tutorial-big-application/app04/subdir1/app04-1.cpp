#include "rodos.h"
#include "common.h"

namespace APP04 {

class AppT1: public Thread {
public:
    AppT1() : Thread("AppT04-1:", 400)  { }
    void run() {
        TIME_LOOP(1200*MILLISECONDS, 1*SECONDS) {
            PRINTF("    APP4-1:\n");
        } // loop
    }

} appT1;

} // namesapce
