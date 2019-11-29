#include "rodos.h"
#include "common.h"

namespace APP03 {

Application application("APP03", 3);

class AppT: public Thread {
public:
    AppT() : Thread("AppT03:", 400)  { }
    void run() {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF("APP3:  counter2: %d\n", Common::counter2);
        } // loop
    }

} appT;

} // namesapce
