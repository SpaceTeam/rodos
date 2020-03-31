
/** Template to handle threads **/

#include "hw_specific.h"
#include "rodos.h"

namespace RODOS {
long* hwInitContext([[gnu::unused]] long* stack, [[gnu::unused]] void* object) { return 0; }
void startIdleThread()    { }

}
extern "C"  {
void __asmSwitchToContext(long* context) { (void)context; }
void __asmSaveContextAndCallScheduler()  { }
}
