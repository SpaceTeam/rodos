#include "thread.h"
#include <ucontext.h>

namespace RODOS {

uintptr_t Thread::getCurrentStackAddr(){
    volatile ucontext_t* c = reinterpret_cast<volatile ucontext_t*>(context);
    return static_cast<uintptr_t>(c->uc_mcontext.gregs[REG_ESP]);
}

}
