#include "rodos.h"

/*This programme shows, how two thread can be executed after each other, first maintest then test1.
 * Each thread MUST first call suspendCallerUntil(); and resume maintest at the end.
 * It is useful to have a unique string before the implementation of each class. With this string each implementation can be easily found using the FIND-function of the text editor (e.g. CTRL+F).
 */

/* Unique Strings to find classes:
 * MainTest:        FIND_MainTest
 * MainTest run:    FIND_MainTest_run
 * Test1:           FIND_Test1
 */

// FIND_MainTest 
class MainTest : public Thread {
    
public:
    
    MainTest() : Thread("MainTest") {}
    void run();
    
}maintest;


// FIND_Test1
class Test1 : public Thread {
    
public:
    
    Test1() : Thread("Test1") {}
    
    void run() {
        suspendCallerUntil();
        PRINTF("TEST1\n");
        maintest.resume();
        suspendCallerUntil();
    }
}test1;

// FIND_MainTest_run
void MainTest::run() {
    PRINTF("MAINTEST\n");
    test1.resume();
    suspendCallerUntil();
    //More classes to be added here. Always classXY.resume; suspendCallerUntil();
    hwResetAndReboot();
}


