

#include "rodos.h"

class MyInitiator : public Initiator {
  public:
    void init() {
        PRINTF(" This will be done before any thread begins\n");
        PRINTF(" Doing so you do not need to use MAIN\n");
    }
} myInitiator;

class MyInitiatorApplic : public Application {
  public:
    MyInitiatorApplic() : Application("initatorApplic", 2000) {}
    void init() {
        PRINTF(" This is the initator from applicasions\n");
        hwResetAndReboot();
    }
} mayInitiatorApplic;
