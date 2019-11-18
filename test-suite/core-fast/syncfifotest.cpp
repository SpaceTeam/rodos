#include "rodos.h"

SyncFifo<int, 10> fifo;

class Sender : public Thread {
    void run() {
        int cnt = 0;
        suspendCallerUntil(NOW() + 0.05 * SECONDS);
        for (int i = 0; i < 100; i++) {
            cnt++;
            bool ok = fifo.syncPut(cnt);
            if(ok) {
                PRINTF("sent %d\n", cnt);
            } else {
                PRINTF("Fifo full\n");
            }
            if((cnt % 20) == 0) {
                PRINTF("Sender Waiting 0.03 seconds\n");
                suspendCallerUntil(NOW() + 0.03 * SECONDS);
            }
        }
        hwResetAndReboot();
    }
};

class Receiver : public Thread {
    void run() {
        int cntIn = 0;
        int cnt   = 0;
        while(1) {
            cnt++;
            bool ok = fifo.syncGet(cntIn);
            if(ok) {
                PRINTF("read %d\n", cntIn);
            }
            if((cnt % 50) == 0) {
                PRINTF("Receiver wait 0.15 seconds\n");
                suspendCallerUntil(NOW() + 0.15 * SECONDS);
            }
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver;
