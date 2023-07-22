#pragma once

#include <cstdint>

#include "hw_interrupt_sync_64bit.h"

namespace RODOS {

template <typename Impl>
class Interruptable_Uint64_Arch : public Impl {
  public:
    Interruptable_Uint64_Arch() : Impl() {}
    Interruptable_Uint64_Arch(uint64_t value) : Impl(value) {}
    Interruptable_Uint64_Arch(const Interruptable_Uint64_Arch& rhs) {
        this->store(rhs.load());
    }

    Interruptable_Uint64_Arch& operator=(uint64_t rhs) {
        this->store(rhs);
        return *this;
    }
    Interruptable_Uint64_Arch& operator=(const Interruptable_Uint64_Arch& rhs) {
        this->store(rhs.load());
        return *this;
    }

    ~Interruptable_Uint64_Arch() = default;

    inline operator uint64_t() const {
        return this->load();
    }

    inline uint64_t load() const {
        return this->loadImpl();
    }

    inline void store(uint64_t value) {
        this->storeImpl(value);
    }
};
using Interruptable_Uint64 = Interruptable_Uint64_Arch<Interruptable_Uint64_Arch_Impl>;


template <typename Impl>
class Interruptable_Int64_Arch : public Impl {
  public:
    Interruptable_Int64_Arch() : Impl() {}
    Interruptable_Int64_Arch(int64_t value) : Impl(value) {}
    Interruptable_Int64_Arch(const Interruptable_Int64_Arch& rhs) {
        this->store(rhs.load());
    }

    Interruptable_Int64_Arch& operator=(int64_t rhs) {
        this->store(rhs);
        return *this;
    }
    Interruptable_Int64_Arch& operator=(const Interruptable_Int64_Arch& other) {
        this->store(other.load());
        return *this;
    }

    ~Interruptable_Int64_Arch() = default;

    inline operator int64_t() const {
        return this->load();
    }

    inline int64_t load() const {
        return this->loadImpl();
    }

    inline void store(int64_t value) {
        this->storeImpl(value);
    }
};
using Interruptable_Int64 = Interruptable_Int64_Arch<Interruptable_Int64_Arch_Impl>;

} // namespace RODOS
