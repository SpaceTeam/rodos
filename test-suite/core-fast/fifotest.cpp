#include "rodos.h"

static Application applic("FifoTest");

Fifo<int, 10> fifo;

class Sender : public StaticThread<> {
    
public:
    
    Sender() : StaticThread<>("Sender", 25) {}
    
    void run() {
        int cnt = 0;
        PRINTF("sender\n");
        for (int i = 0; i < 100; i++) {
            cnt++;
            bool ok = fifo.put(cnt);
            if(ok) {
                PRINTF("Sending %d\n", cnt);
            } else {
                PRINTF("Fifo full\n");
            }
            if((cnt % 15) == 0) {
                PRINTF("Wainting 0.03 seconds\n");
                suspendCallerUntil(NOW() + 0.03 * SECONDS);
            }
        }
        hwResetAndReboot();
    }
};

class Receiver : public StaticThread<> {

public:
    
    Receiver() : StaticThread<>("Receiver", 10) {}
    
    void run() {
        int cnt;
        PRINTF("receiver\n");

        while(1) {
            bool ok = fifo.get(cnt);
            if(ok) {
                PRINTF("reading %d\n", cnt);
            } else {
                suspendCallerUntil(NOW() + 0.01 * SECONDS);
            }
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver;
