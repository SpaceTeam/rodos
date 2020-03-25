#include "rodos.h"

#include "commbuffer.h"

class CommBufferTester : public StaticThread<> {

  public:
    void run() {
       CommBuffer<int32_t> buff;
       int tmp;
       
       buff.put(5);
       buff.get(tmp);
       PRINTF("%d \n", tmp);        //MUST print 5
       
       if(buff.getOnlyIfNewData(tmp)) {
           PRINTF("MAY NOT happen\n");
        } else {
            PRINTF("No new data\n");     //Must end here
        }
        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();

    }
} commbufferTester;

