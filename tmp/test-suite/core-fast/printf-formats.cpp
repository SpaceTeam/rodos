#include "rodos.h"


class TestPrint : public Thread {
  public:
    void run() {

        PRINTF(" Showing differnt printf formats\n\n\n");

        float  a = 123456.789;
        float  b = -0.00000000123456789;
        double c;
        *(int64_t*)&c = 0x7FF8000000000000; // nan

        PRINTF(" as f : %f     %3.15f    %f\n", a, b, c);
        PRINTF(" as e : %e     %2.5e     %e\n", a, b, c);

        PRINTF("\n\n");
        PRINTF("as d:  %d\n", -12);
        PRINTF("as u:  %u\n", -12);
        PRINTF("as o:  %o\n", 12);
        PRINTF("as x:  %04x\n", 12);
        PRINTF("as b:  %08b\n", 0x55); // please ignore the c++ compile warning

        PRINTF("\n\n");
        PRINTF("as s: '%s'\n", "this is a string");
        
        hwResetAndReboot();
    }

} testPrint;
