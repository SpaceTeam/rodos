
/** timer template **/

#include "rodos.h"
#include "hw_specific.h"

namespace RODOS {
long long Timer::microsecondsInterval = 0;

void Timer::init()  { }
void Timer::start() { }
void Timer::stop()  { }
void Timer::setInterval([[gnu::unused]] const long long microsecondsInterval) { }


}
