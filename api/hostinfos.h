
/**
* @file hostinfos.h
* @author Sergio Montenegro
*/

#pragma once

/**  bigendian is standard net protocol. 
 * Warning: Will be set in main.
 * never use before main (in static constructors)
 **/


namespace RODOS {

extern bool  getIsHostBigEndian();          ///< never use before main()
extern long  getSpeedKiloLoopsPerSecond();  ///< only an approximation, max differ +/- 50%
extern long  getMaxIdleCntPerSecond();      // see rodos-tutorials/development-tests/cpuspeedtest.cpp
extern long  getYieldTimeOverhead();        ///< number of nanoseconds for a thread switch
extern const char* getHostCpuArch(); 
extern const char* getHostBasisOS();

extern volatile int64_t idleCnt; ///< a hint about cpu idle time, cnt will be incremented by idleThread

} // namespace

