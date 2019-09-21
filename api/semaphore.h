



/**
* @file semaphore.h
* @date 2008/05/10 16:37
* @author Sergio Montenegro
*
*
* @breif binary semaphore, mutex a synchronisation primitive 
*/

#pragma once

#include "thread.h"

namespace RODOS {

/**
* @class Semaphore
* @brief a synchronisation primitive
*
* Mutex Semaphore.
* the next thread to be activated is
* selected according to priorities and
* not come first serve first.
*
*/

class Semaphore {

private:
  Thread* volatile owner;
  volatile int ownerEnterCnt;

protected:
  volatile long ownerPriority;
  void* context; ///< used only on posix and on host-os

public:
  /** Constructor */
  Semaphore();
  // ~Semaphore() { } // Shall never be called. Semaphores may not disappear

  /** caller will be blocked if semaphore is occupied 
   ** The owner may reenter the semaphore without deadlock */
  void enter();

  /** caller does not block. Resumes one waiting thread (enter) */
  void leave();

  /** true if semaphore is free:
  *   Warning: next it can be occupied by someone else
  */
  // bool isFree();
 
  /// This is just a help for the macro PROTECT_WITH_SEMAPHORE (DEPRECATED)
  // inline bool enterRetTrue() { enter(); return true; }
};

// Use this class to protect a scope with a semaphore.
// Just create an instance on the stack; it immediately enters a given semaphore.
// On destruction it leaves the semaphore.
class ScopeProtector {
  Semaphore *sema;
  bool left = false;

public:
  explicit ScopeProtector(Semaphore *const inputSema) : sema(inputSema) { sema->enter(); }
  ~ScopeProtector() { leave(); }

  // If you want to leave the semaphore preliminary, use this method.
  void leave() { if(!left) sema->leave(); left = true; }
};

#define PROTECT_IN_SCOPE(_sema) ScopeProtector _scope_protector_(&(_sema));

#if 0
// DEPRECATED! use PROTECT_IN_SCOPE instead
#define PROTECT_WITH_SEMAPHORE(sem) \  !!!Commented out: DEPRECATED!!!
  for(bool _onlyonce_ = sem.enterRetTrue(); _onlyonce_ ; _onlyonce_ = false, sem.leave())
#endif

} // namespace

