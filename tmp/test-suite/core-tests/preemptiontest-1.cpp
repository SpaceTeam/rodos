#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

static int prio = 1;        //Priorität des Threads, priority ist bereits in Rodos vergeben

class LowPriorityThread : public Thread {
    char symbol;

  public:
    LowPriorityThread(char symbol) : Thread("LowPriority", prio) {
        this->symbol = symbol;
        prio+=10;
    }

    void init() {
        xprintf(" lopri = '.'");
    }

    void run() {
        volatile int64_t cnt             = 0;
        int64_t          intervalToPrint = 10;
        for (int i = 0; i < 100; i++) {
            cnt++;
            if(cnt % intervalToPrint == 0) {
                xprintf("%c", symbol);
                FFLUSH();
            }
        }
    }
};


/******************/

LowPriorityThread lp1('.');
LowPriorityThread lp2('+');
LowPriorityThread lp3('-');
LowPriorityThread lp4('*');


/******************/
