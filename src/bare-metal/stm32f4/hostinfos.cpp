
/** READ THIS!!!! **/
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include "rodos.h"

namespace RODOS {

bool  getIsHostBigEndian()          { return isHostBigEndian; }

long  getSpeedKiloLoopsPerSecond()  { return 6995;  } // see .../development-tests/cspuspeedtest.cpp
long  getMaxIdleCntPerSecond()      { return 70000; } // see rodos-tutorials/development-tests/cpuspeedtest.cpp
long  getYieldTimeOverhead()        { return 50000; } // see tutorials/core/yieldtime.cpp
const char* getHostCpuArch()        { return "cortex-M4 (STM32F4)";    }
const char* getHostBasisOS()        { return "baremetal"; }
}
