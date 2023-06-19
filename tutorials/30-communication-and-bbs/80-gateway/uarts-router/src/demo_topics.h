#include "rodos.h"

struct myDataType {
    int  cnt;
    char stringMsg[16];
};

extern Topic<myDataType> topic1;
extern Topic<myDataType> topic2;
