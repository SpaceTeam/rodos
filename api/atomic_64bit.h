#pragma once

#include <cstdint>

#include "interrupt_sync_64bit.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

class Atomic_Uint64 {
  public:
    Atomic_Uint64(uint64_t value) : m_value{ value } {}

    Atomic_Uint64(const Atomic_Uint64& rhs) {
        this->store(rhs.load());
    }
    Atomic_Uint64& operator=(const Atomic_Uint64& other) {
        this->store(other.load());
        return *this;
    }
    ~Atomic_Uint64() = default;

    operator uint64_t() const {
        return this->load();
    }

    Interruptable_Uint64& raw() {
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
    Interruptable_Uint64 m_value;
};

class Atomic_Int64 {
  public:
    Atomic_Int64(int64_t value) : m_value{ value } {}

    Atomic_Int64(const Atomic_Int64& rhs) {
        this->store(rhs.load());
    }
    Atomic_Int64& operator=(const Atomic_Int64& other) {
        this->store(other.load());
        return *this;
    }
    ~Atomic_Int64() = default;

    operator int64_t() const {
        return this->load();
    }

    Interruptable_Int64& raw() {
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
    Interruptable_Int64 m_value;
};


} // namespace RODOS
