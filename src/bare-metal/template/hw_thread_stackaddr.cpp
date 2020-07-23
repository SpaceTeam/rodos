#include "thread.h"

namespace RODOS {

uintptr_t Thread::getCurrentStackAddr(){
    return reinterpret_cast<uintptr_t>(context);
}

}
