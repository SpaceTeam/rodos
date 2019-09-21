#include "rodos.h"

// Just prints the address of a variable in global memory

uint8_t  var1 = 123;
uint32_t var2 = 456;
uint64_t var3 = 789;
uint64_t var4 = 000;

static Application application("MemTest");

class MemTest : public Thread {

  public:
    // Constructor
    MemTest() : Thread("mem-test") {}


    void init() {
        PRINTF("Mem Test: Just displaying mem locations of variables\n");
    }

    void run() {
        PRINTF("Mem Test: run\n");
        PRINTF("var1: %d@%x\n", (int)var1, (int)&var1);
        PRINTF("var2: %d@%x\n", (int)var2, (int)&var2);
        PRINTF("var3: %d@%x\n", (int)var3, (int)&var3);
        PRINTF("var4: %d@%x\n", (int)var4, (int)&var4);

        uint16_t cnt = 0;
        uint64_t t0;
        while(1) {
            cnt++;
            suspendCallerUntil(NOW() + 2 * SECONDS);
            t0 = NOW();
            PRINTF("Time now: %lld - cnt: %d\n", t0, cnt);
        }
    }
};

static MemTest memTest;
//static MemTest memTest2;
