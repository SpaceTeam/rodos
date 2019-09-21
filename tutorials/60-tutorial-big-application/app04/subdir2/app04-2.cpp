#include "rodos.h"
#include "common.h"

namespace APP04 {

class AppT2: public Thread {
public:
    AppT2() : Thread("AppT04-2:", 400)  { }
    void run() {
        TIME_LOOP(1300*MILLISECONDS, 1*SECONDS) {
            PRINTF("    APP4-2:\n");
        } // loop
    }

} appT2;

} // namesapce
