#include "rodos.h"
#include "common.h"

namespace APP01 {

class AppT2: public Thread {
public:
    AppT2() : Thread("AppT1-2:", 400)  { }
    void run() {
        TIME_LOOP(1500*MILLISECONDS, 1*SECONDS) {
            PRINTF("APP1:  01-02\n");
            Common::counter2++;
        } // loop
    }

} appT2;

} // namesapce
