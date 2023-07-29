#pragma once

#include <atomic>
#include <cstddef>

namespace RODOS {

/**
 * @brief Generic atomic variable wrapper that is guaranteed to be atomic in RODOS scheduling
 * as long as only one thread is reading and/or writing to the variable (arbitrarily many
 * interrupts may read and/or write to the variable).
 *
 * Load/store operations exist that are reentrant (async-signal-safe).
 * - A load operation is guaranteed to yield a valid value (that the variable had at a time
 * during the function call).
 * - A store operation is guaranteed to have stored the new value completely after it returns.
 *
 * @note Operations also support interrupts interrupting each other (still async-safe).
 *
 * @warning Load/store operations are not thread-safe! When multiple threads need to access
 * the variable, operations need to be protected via Semaphores.
 *
 * @tparam T Type of the stored shared variable.
 */
template <typename T>
class Generic_Atomic_1_ThreadRW_M_InterruptRW {
  public:
    Generic_Atomic_1_ThreadRW_M_InterruptRW(const T& value) {
        m_valueBuffer[m_index.load()] = value;
    }

    Generic_Atomic_1_ThreadRW_M_InterruptRW(
      const Generic_Atomic_1_ThreadRW_M_InterruptRW& rhs) {
        this->store(rhs.load());
    }
    Generic_Atomic_1_ThreadRW_M_InterruptRW& operator=(
      const Generic_Atomic_1_ThreadRW_M_InterruptRW& other) {
        this->store(other.load());
        return *this;
    }
    ~Generic_Atomic_1_ThreadRW_M_InterruptRW() = default;

    operator T() const {
        return this->load();
    }

    /**
     * @brief Load (copy) variable in an async-safe manner (but not thread-safe).
     *
     * Tries to load the value as long as the operation keeps getting interrupted and the
     * interrupt handler stores a new value. This is necessary as in such a case the loaded value
     * might be invalid (half old, half new).
     */
    T load() const {
        T temporaryValue{};
        do {
            m_isLoadUninterrupted.store(true);
            temporaryValue = m_valueBuffer[m_index.load()];
        } while(loadWasInterruptedByStore());
        return temporaryValue;
    }

    /**
     * @brief Store new value to variable in an async-safe manner (but not thread-safe).
     *
     * Tries to store the value as long as the operation keeps getting interrupted and the
     * interrupt handler stores a new value. This is necessary as in such a case the stored value
     * might be invalid (either wrong index or half new, half old).
     *
     * At the end, the operation notifies other potentially interrupted operations about the store
     * via an atomic flag.
     */
    void store(const T& newValue) {
        do {
            m_isLoadUninterrupted.store(true);
            copyAndSwapValueBuffer(newValue);
        } while(loadWasInterruptedByStore());
        notifyLoadOperation();
    }

  private:
    inline void copyAndSwapValueBuffer(const T& newValue) {
        uint8_t newIndex        = (m_index.load() == 0) ? 1 : 0;
        m_valueBuffer[newIndex] = newValue;
        m_index.store(newIndex);
    }

    inline void notifyLoadOperation() const {
        m_isLoadUninterrupted.store(false);
    }

    inline bool loadWasInterruptedByStore() const {
        return (m_isLoadUninterrupted.load() == false);
    }

    volatile T                m_valueBuffer[2]{};
    std::atomic<uint8_t>      m_index{ 0 };
    mutable std::atomic<bool> m_isLoadUninterrupted{ false };
};

} // namespace RODOS
