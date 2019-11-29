#include "rodos.h"
#include "common.h"

namespace APP01 {

Application application("APP01", 1);

class AppT1: public Thread {
public:
    AppT1() : Thread("AppT1-1:", 400)  { }
    void run() {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF("APP1:  01-01\n");
            Common::counter1++;
        } // loop
    }

} appT1;

} // namesapce
