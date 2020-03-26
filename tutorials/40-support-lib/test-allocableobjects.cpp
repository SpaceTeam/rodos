
#include "rodos.h"
#include "support-libs.h"


struct TestObj : SortedChainable {
    int a, b, c;
};

AllocableObejcts<TestObj, 20> allocableTestObjs;


class TestAllocs : public StaticThread<> {
  public:
    TestObj* tmp[300];
    void     run();
} testAllocs;

void TestAllocs::run() {
    allocableTestObjs.init();
    PRINTF("free object %d\n", allocableTestObjs.getNumOfFreeItems());

    PRINTF("allocate\n");
    for(int i = 0; i < 25; i++) {
        tmp[i] = allocableTestObjs.alloc();
        PRINTF("    index %d, free = %d\n", allocableTestObjs.getIndex(tmp[i]), allocableTestObjs.getNumOfFreeItems());
    }

    PRINTF("copy references\n");
    for(int i = 0; i < 10; i++) {
        tmp[i + 25] = allocableTestObjs.copyReference(tmp[i]);
        PRINTF("    index %d, free = %d\n", allocableTestObjs.getIndex(tmp[i + 25]), allocableTestObjs.getNumOfFreeItems());
    }

    PRINTF("See List\n");
    for(int i = 0; i < 40; i++) {
        PRINTF("    index %d\n", allocableTestObjs.getIndex(tmp[i]));
    }

    PRINTF("free\n");
    for(int i = 0; i < 40; i++) {
        allocableTestObjs.free(tmp[i]);
        PRINTF("    index %d, free = %d\n", allocableTestObjs.getIndex(tmp[i]), allocableTestObjs.getNumOfFreeItems());
    }
}
