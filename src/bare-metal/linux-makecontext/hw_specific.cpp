
/**
 * @file hw_specific.cc
 * @date 2008/04/23 7:33
 * @author Johanes Freitag, Sergio Montenegro
 *
 * Uni Wuerzburg
 *
 * @brief all hardware specific stuff
 *
 */

#include "hw_specific.h"
#include "rodos.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> // only for nanosleep
#include <ucontext.h> // for makecontext & co

namespace RODOS {

ucontext_t* volatile contextT;

#define STACKSIZE 4096     /* stack size  WARNING/TODO: That is this? */
ucontext_t signal_context; /* the interrupt context */
void*      signal_stack;   /* global interrupt stack */

/**
 * initialize hardware specific stuff
 */

extern void sigtermHandler(int sig);
extern int32_t myNodeNr;

//_____________________________________________________
void hwInit() {
    taskRunning = 0;
    signal(SIGTERM, sigtermHandler); // External interrupts simulator
    myNodeNr = getpid(); // a default node number, may be replaced by the link interface (gateway)
}

//_____________________________________________________
void hwResetAndReboot() {
    isShuttingDown = true;
    xprintf("hw_resetAndReboot() -> exit\n");
    exit(0);
}

//_____________________________________________________
// cpu core and periferis off until extern interrupt or time "until"
void deepSleepUntil(int64_t until) {
    int64_t  deltaT = until - NOW();
    timespec deltaTPosix;
    timespec remainingTime;
    deltaTPosix.tv_sec  = static_cast<__time_t>(deltaT / SECONDS);
    deltaTPosix.tv_nsec = static_cast<__syscall_slong_t>(deltaT % SECONDS);

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
void hwInitWatchdog(int32_t) { }
void hwTriggerWatchdog() { timeOfLastTriggerWatchdog = NOW(); }

//_____________________________________________________
/**
 *create context on stack and return a pointer to it
 */
long* hwInitContext(long* stack, void* object) {
    contextT = (ucontext_t*)xmalloc(sizeof(ucontext_t));

    getcontext(contextT);
    contextT->uc_stack.ss_sp    = stack;
    contextT->uc_stack.ss_size  = sizeof stack;
    contextT->uc_stack.ss_flags = 0;
    if(sigemptyset(&contextT->uc_sigmask) < 0) {
        perror("sigemptyset");
        isShuttingDown = true;
        exit(1);
    }
    makecontext(contextT, (void (*)())threadStartupWrapper, 1, object);

    return reinterpret_cast<long*>(contextT);
}

void sp_partition_yield() { }
void enterSleepMode()     { }
void startIdleThread()    { }
void FFLUSH()             { fflush(stdout); }

} // namespace RODOS


//___________________________________________________________________________
/*
 *  In other bare metal implementations this has to be done in assembler 
 */
extern "C" void __asmSaveContextAndCallScheduler();
void __asmSaveContextAndCallScheduler() {

    /* Create new scheduler context */
    getcontext(&signal_context);
    signal_context.uc_stack.ss_sp    = signal_stack;
    signal_context.uc_stack.ss_size  = STACKSIZE;
    signal_context.uc_stack.ss_flags = 0;
    sigemptyset(&signal_context.uc_sigmask);
    makecontext(&signal_context, (void (*)())schedulerWrapper, 1, contextT);

    /* save running thread, jump to scheduler */
    swapcontext(contextT, &signal_context);
}

extern "C" void __asmSwitchToContext(long* context);
void __asmSwitchToContext(long* context) {
    contextT = (ucontext_t*)context;
    setcontext(contextT); /* go */
}

