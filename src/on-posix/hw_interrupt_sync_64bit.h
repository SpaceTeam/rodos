#pragma once

#include <atomic>

namespace RODOS {

class Interruptable_Uint64_Posix_Impl {
  public:
    Interruptable_Uint64_Posix_Impl() : m_value{} {}
    Interruptable_Uint64_Posix_Impl(uint64_t value) : m_value{ value } {}

  protected:
    inline uint64_t loadImpl() const {
        return m_value.load();
    }

    inline void storeImpl(uint64_t value) {
        m_value.store(value);
    }

  private:
    std::atomic<uint64_t> m_value;
};


class Interruptable_Int64_Posix_Impl {
  public:
    Interruptable_Int64_Posix_Impl() : m_value{} {}
    Interruptable_Int64_Posix_Impl(int64_t value) : m_value{ value } {}

  protected:
    inline int64_t loadImpl() const {
        return m_value.load();
    }

    inline void storeImpl(int64_t value) {
        m_value.store(value);
    }

  private:
    std::atomic<int64_t> m_value;
};

using Interruptable_Uint64_Arch_Impl = Interruptable_Uint64_Posix_Impl;
using Interruptable_Int64_Arch_Impl  = Interruptable_Int64_Posix_Impl;

} // namespace RODOS
