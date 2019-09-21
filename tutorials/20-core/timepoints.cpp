/**
 * @file timepoints.cc
 * @date 2008/06/12 10:10
 * @author Lutz Dittrich
 *
 *
 * @brief test for class TimePoints
*/
#include "rodos.h"

static Application module01("TestTimePoints");

class TestThread : public Thread {

  private:
    TimePoints<30> tp;

  public:
    TestThread(const char* name = "TestThread") : Thread(name) {}

    void run() {
        int i = 0;
        tp.add("run");
        tp.add("next entry");
        tp.printAll(); tp.clear();
        while(1) {
            if(i > 20) {
                tp.printAll();
                tp.clear();
                errorLog.printAll();
                i = 0;
            }
            i++;
            tp.add(" loop entry");
            suspendCallerUntil(NOW() + 100 * MILLISECONDS);
        }
    }
};

TestThread t;
