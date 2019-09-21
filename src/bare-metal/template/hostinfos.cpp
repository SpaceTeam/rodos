
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include "rodos.h"

namespace RODOS {


long  getSpeedKiloLoopsPerSecond()  { return 6995;  } // see tutorials/20-core/compute-hostinfos.cpp
long  getMaxIdleCntPerSecond()      { return 70000; } // see tutorials/20-core/compute-hostinfos.cpp
long  getYieldTimeOverhead()        { return 50000; } // see tutorials/20-core/compute-hostinfos.cpp

bool  getIsHostBigEndian()          { return isHostBigEndian; }  // Yes! Actually getIsHostBigEndian is host independent
const char* getHostCpuArch()        { return "template";    }
const char* getHostBasisOS()        { return "baremetal"; }
}
