
/** READ THIS!!!! **/
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp  and core/yieldtime.cpp**/

#include "rodos.h"
namespace RODOS {
bool  getIsHostBigEndian()          { return isHostBigEndian; }
long  getSpeedKiloLoopsPerSecond()  { return 313330;      } // see rodos-tutorials/development-tests/cpuspeedtest.cpp
long  getMaxIdleCntPerSecond()      { return 4961425;     } // see rodos-tutorials/development-tests/cpuspeedtest.cpp
long  getYieldTimeOverhead()        { return 990;         } // see tutorials/core/yieldtime.cpp
const char* getHostCpuArch()        { return "x86";       } 
const char* getHostBasisOS()        { return "baremetal"; }
}
