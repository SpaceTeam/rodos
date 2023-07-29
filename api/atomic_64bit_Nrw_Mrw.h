#pragma once

#include <cstdint>

#include "atomic_64bit_Nro_1rw.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

/**
 * @name 64bit variable wrappers that are guaranteed to be atomic in RODOS scheduling.
 * Arbitrarily many threads can read and/or write to the variable and
 * arbitrarily many interrupts can read and/or write to the variable.
 *
 * @warning The store operation temporarily disables all interrupts. Use with care!
 * You can access the raw underlying Atomic_N_ThreadRO_1_InterruptRW variable which does not
 * disable interrupts during any operation but is more restrictive via raw().
 *
 * @{
 */
class Uint64_Atomic_N_ThreadRW_M_InterruptRW {
  public:
    Uint64_Atomic_N_ThreadRW_M_InterruptRW(uint64_t value) : m_value{ value } {}

    Uint64_Atomic_N_ThreadRW_M_InterruptRW(
      const Uint64_Atomic_N_ThreadRW_M_InterruptRW& rhs) {
        this->store(rhs.load());
    }
    Uint64_Atomic_N_ThreadRW_M_InterruptRW& operator=(
      const Uint64_Atomic_N_ThreadRW_M_InterruptRW& other) {
        this->store(other.load());
        return *this;
    }
    ~Uint64_Atomic_N_ThreadRW_M_InterruptRW() = default;

    operator uint64_t() const {
        return this->load();
    }

    Uint64_Atomic_N_ThreadRO_1_InterruptRW& raw() {
        return m_value;
    }

    uint64_t load() const {
        return m_value.load();
    }

    void store(uint64_t value) {
        hwDisableInterrupts();
        m_value.store(value);
        hwEnableInterrupts();
    }

  private:
    Uint64_Atomic_N_ThreadRO_1_InterruptRW m_value;
};

class Int64_Atomic_N_ThreadRW_M_InterruptRW {
  public:
    Int64_Atomic_N_ThreadRW_M_InterruptRW(int64_t value) : m_value{ value } {}

    Int64_Atomic_N_ThreadRW_M_InterruptRW(
      const Int64_Atomic_N_ThreadRW_M_InterruptRW& rhs) {
        this->store(rhs.load());
    }
    Int64_Atomic_N_ThreadRW_M_InterruptRW& operator=(
      const Int64_Atomic_N_ThreadRW_M_InterruptRW& other) {
        this->store(other.load());
        return *this;
    }
    ~Int64_Atomic_N_ThreadRW_M_InterruptRW() = default;

    operator int64_t() const {
        return this->load();
    }

    Int64_Atomic_N_ThreadRO_1_InterruptRW& raw() {
        return m_value;
    }

    int64_t load() const {
        return m_value.load();
    }

    void store(int64_t value) {
        hwDisableInterrupts();
        m_value.store(value);
        hwEnableInterrupts();
    }

  private:
    Int64_Atomic_N_ThreadRO_1_InterruptRW m_value;
};
/** @} */

} // namespace RODOS
