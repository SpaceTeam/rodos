

#include "rodos.h"

static class MyInitiator : public Initiator {
    void init() {
        PRINTF(" This will be done before any thread begins\n");
        PRINTF(" Doing so you do not need to use MAIN\n");
        hwResetAndReboot();
    }
} myInitiator;
