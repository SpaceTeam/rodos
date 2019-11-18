#include "rodos.h"


// versuchen auch mit priritaeten 30 20 10

int64_t counter = 0;
static const int64_t COUNT_LIMIT = 1000 * 1000 * 100;

static const int NUM_OF_THREADS = 20;
int threadsFinisched = 0;

Semaphore protector;

class CountThread : public Thread {
public:
  CountThread() : Thread() { }
  void run() {
    protector.enter();
    for(int64_t i = 0; i < COUNT_LIMIT; i++) {
        counter++;
    }
    threadsFinisched++;
    protector.leave();
    if(threadsFinisched >=  NUM_OF_THREADS) {
        PRINTF(" %d from %d Threads finished -> terminate\n", threadsFinisched, NUM_OF_THREADS);
        PRINTF("counter am Ende = %lld\n", counter);
        hwResetAndReboot();
    }
  }
};


CountThread ct[NUM_OF_THREADS];


