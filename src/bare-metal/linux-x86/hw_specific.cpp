
/**
 * @file hw_specific.cc
 * @date 2008/04/23 7:33
 * @author Sergio Montenegro, Lutz Dittrich
 *
 * Uni Wuerzburg
 *
 * @brief all hardware specific stuff
 *
 */

#include "context.h"
#include "hw_specific.h"
#include "rodos.h"

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> // only for nanosleep

extern "C" void abort();

namespace RODOS {

/**
 * initialize hardware specific stuff
 */

extern void sigtermHandler(int sig);
extern long myNodeNr;

//_____________________________________________________
void hwInit() {
    taskRunning = 0;
    signal(SIGTERM, sigtermHandler); // External interrupts simulator
    myNodeNr = getpid();             // a default node number, may be replaced by the link interface (gateway)
}

//_____________________________________________________
void hwResetAndReboot() {
    xprintf("hw_resetAndReboot() -> exit");
    abort();
}

//_____________________________________________________
// cpu core and periferis off until extern interrupt or time "until"
void deepSleepUntil(int64_t until) {
    int64_t  deltaT = until - NOW();
    timespec deltaTPosix;
    timespec remainingTime;
    deltaTPosix.tv_sec  = deltaT / SECONDS;
    deltaTPosix.tv_nsec = deltaT % SECONDS;

    hwDisableInterrupts();
    nanosleep(&deltaTPosix, &remainingTime);
    hwEnableInterrupts();
}

//_____________________________________________________
sigset_t oldset;
void hwDisableInterrupts() {
    sigset_t newset;
    sigfillset(&newset);
    sigprocmask(SIG_SETMASK, &newset, &oldset);
}
void hwEnableInterrupts() {
    sigprocmask(SIG_SETMASK, &oldset, 0);
}

//_____________________________________________________
static int64_t timeOfLastTriggerWatchdog = 0;
void hwInitWatchdog(long intervalMilliseconds) {}
void hwTriggerWatchdog() { timeOfLastTriggerWatchdog = NOW(); }

//_____________________________________________________
/**
 *create context on stack and return a pointer to it
 */
long* hwInitContext(long* stack, void* object) {
    long* tos     = stack; /* top of stack */
    *tos--        = (long)object;
    *tos          = 0;
    TContext* ctx = (TContext*)(tos - sizeof(TContext) / sizeof(long));
    ctx->EIP      = (long)(threadStartupWrapper);
    ctx->ESP      = (long)stack;
    __asm__ __volatile__("pushf \n"
                         "\tpop %0"
                         : "=r"(ctx->EFLAGS));
    /* save current floating point context */
    __asm__ __volatile__("fnsave (%0)" ::"r"(ctx->floatingPointContext));
    /*  xprintf("init_context: stack=%08lX ctx=%08lX\n",(long)stack,(long)ctx); */
    return (long*)ctx;
}
//_____________________________________________________
void sp_partition_yield() { }
void enterSleepMode()     { }
void startIdleThread()    { }
void FFLUSH()             { fflush(stdout); }

} // namespace RODOS
