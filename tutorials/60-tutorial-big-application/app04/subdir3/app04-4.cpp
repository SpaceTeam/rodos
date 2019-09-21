#include "rodos.h"
#include "common.h"

namespace APP04 {

class AppT4: public Thread {
public:
    AppT4() : Thread("AppT04-4:", 400)  { }
    void run() {
        TIME_LOOP(1500*MILLISECONDS, 1*SECONDS) {
            PRINTF("    APP4-4:\n");
        } // loop
    }

} appT4;

} // namesapce
