#include "rodos.h"

#include "fifo.h"

class FiFoTester : public Thread {

  public:
    void run() {
        Fifo<int32_t, 3> fifo; //A maximum of 3-1=2 ints in the Fifo
        int              tmp;
        bool             ok;

        ok = fifo.put(1);
        PRINTF("1. write success %d\n", ok);

        ok = fifo.get(tmp);
        PRINTF("read success %d, data  %d\n", ok, tmp);

        ok = fifo.get(tmp);
        PRINTF("read success %d (expected 0)\n", ok);

        ok = fifo.isEmpty();
        PRINTF("is empty  %d\n", ok);

        ok = fifo.put(2);
        PRINTF("2. write success %d\n", ok);
        ok = fifo.put(3);
        PRINTF("3. write success %d\n", ok);
        ok = fifo.put(4);
        PRINTF("4. write success %d (expected 0)\n", ok);

        for(int i = 0; i < 5; i++) {
            ok = fifo.get(tmp);
            PRINTF("read success %d, data %d\n", ok, tmp);
        }

        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();
    }
} fifoTester;
