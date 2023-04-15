#include <cstdint>

#include "thread.h"
#include "timeevent.h"
#include "yprintf.h"

namespace RODOS {

constexpr auto INTERVAL = 1250 * MILLISECONDS;

class DummyThread : public StaticThread<> {
  public:
    DummyThread(int32_t prio, int64_t start)
      : StaticThread<>("dummyThread", prio), m_start(start) {}

    void run() {
        TIME_LOOP(m_start, INTERVAL) {
            busyWaitUntil(TimeModel::computeNextBeat(0, INTERVAL / 2, NOW()));
        }
    }

  private:
    int64_t m_start;
};

DummyThread dummyThread1{ 100, 0 * MILLISECONDS };
DummyThread dummyThread2{ 200, 200 * MILLISECONDS };

class PeriodicEvent : public TimeEvent {
  protected:
    void handle() {
        xprintf("> %3.9f\n", SECONDS_NOW());
    }

    void init() {
        this->activatePeriodic(0, INTERVAL);
    }
} periodicEvent{};

} // namespace RODOS
