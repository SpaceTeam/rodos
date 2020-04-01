
/**
* @file misc-rodos-funcs.h
* @date 2008/06/17 10:46
* @author Sergio MOntenegro
*
* @brief simple misc functions
*/

#pragma once


#include <stdint.h>
#include <stddef.h>


namespace RODOS {

extern bool isShuttingDown; // set by hwResetAndReboot(), read by destructors

/**
 * if a network and a gateway are present, my node number
 */
long getNodeNumber();

int64_t getNumberOfReceivedMsgsFromNetwork();

/**
 * allocation of memory. No free of memory exists.
 * allocates static memory.
 * it shall not be unset after thread:run() begin
 */
extern void* xmalloc(size_t len);

extern void hwResetAndReboot();        ///<  End of Program -> reboot Hw dependent
extern void hwInitWatchdog(long intervalMilliseconds);
extern void hwTriggerWatchdog();        ///<  for CPUS which provide a hardware watchdog
extern void hwDisableInterrupts();      // global interrupt disable - use carefully
extern void hwEnableInterrupts();       // global interrupt enable - use carefully
extern void deepSleepUntil(int64_t until); //< cpu core and periphery off until external interrupt or time "until"
extern bool isSchedulerRunning();       //< implemented in the platform dependent scheduler


/** Nop... no operation ... do nothing ***/
void nop( ... );

extern float getCpuLoad(); ///< value from 0 to 1, average from the last call


inline int getbit(uint32_t bitmap, int bitIndex) { return (bitmap >> bitIndex) & 0x01; }

inline void setbit(uint32_t* bitmap, int value, uint8_t bitIndex) {
    if(value) *bitmap |=  (0x01u  << bitIndex);
    else      *bitmap &= ~(0x01u  << bitIndex);
}


}  // namespace


/**
 * MAIN() will be executed after initialisation and before
 * threads are started. RODOS provides an empty MAIN function.
 * The user may write its own MAIN function which will be
 * linked instead of the RODOS empty one.
 * outside of namespace RODOS!
 **/
void MAIN();

