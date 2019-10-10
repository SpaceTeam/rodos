#include "rodos.h"

#include "../prt-seconds-now.h"

static int prio = 1;        //Priorität des Threads, priority ist bereits in Rodos vergeben

static Application module("semaphoretest");

Semaphore protector;

class TestThread : public Thread {
public:
    
    void run() {
        for (int i = 0; i < 1000; i++) {
            xprintf("%s\n", getName());
            yield();
            xprintf("%s before protector \n", getName());

            {
                PROTECT_IN_SCOPE(protector);

                xprintf("------- %s entered at %3.9f\n", getName(), CNT_SECONDS_NOW());
                yield();
                xprintf("          %s 1.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                xprintf("          %s 2.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                xprintf("          %s 3.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                xprintf("          %s 4.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                xprintf("-------- %s leavs getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            }


            xprintf("%s after protector getScheduleCounter()  %lld \n", getName(), getScheduleCounter());
            yield();
        }
    }

    TestThread(const char* name) : Thread(name, prio) {prio++;}
};


TestThread t1("aaa");
TestThread t2("bbb");
TestThread t3("ccc");
TestThread t4("ddd");
