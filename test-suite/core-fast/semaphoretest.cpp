#include "rodos.h"

#include "../prt-seconds-now.h"

static Application module("semaphoretest");

Semaphore protector;

static int prio = 1;        //Priorität des Threads, priority ist bereits in Rodos vergeben

class TestThread : public StaticThread<> {

public:
    TestThread() : StaticThread<>("TestThread", prio) {prio += 10;}
    
    void run() {
        for (int i = 0; i < 25; i++) {
            xprintf("%s\n", getName());
            yield();
            xprintf("%s before protector \n", getName());
            protector.enter();

            xprintf("------- %s entered at %3.9f \n", getName(), CNT_SECONDS_NOW());
            yield();
            xprintf("          %s 1.yield. getScheduleCounter()  %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 2.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 3.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 4.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("-------- %s leavs getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            protector.leave();


            xprintf("%s after protector getScheduleCounter()  %lld \n", getName(), getScheduleCounter());
            yield();
        }
        hwResetAndReboot();
    }

  public:
    TestThread(const char* name) : StaticThread<>(name) {}
};


TestThread t1("aaa");
TestThread t2("bbb");
TestThread t3("ccc");
TestThread t4("ddd");
