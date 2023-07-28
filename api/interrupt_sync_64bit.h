#pragma once

#include <cstdint>

#include "hw_interrupt_sync_64bit.h"

namespace RODOS {

namespace detail {

    /**
     * @name Wrapper for an interruptable signed/unsigned 64bit integer variable.
     *
     * Operations on it may not be atomic themselves, however, operations are
     * guaranteed to only return in case they have not been interrupted.
     *
     * @warning This is not an atomic variable! It does not guarantee that interrupts or
     * other threads might see partial writes!
     * @warning Do not use this on MMIO/peripherals as it might spuriously perform multiple writes!
     *
     * @note Shouldn't be used directly, use the architecture-agnostic typedefs instead.
     * @see Interruptable_Uint64 Interruptable_Int64
     *
     * @tparam Impl Architecture-dependent implementation of the interruptable double word variable.
     * Uses version of "mixin" pattern, used to add common constructors and ensure unified interface.
     *
     * @{
     */
    template <typename Impl>
    class Interruptable_Uint64_Arch : public Impl {
      public:
        Interruptable_Uint64_Arch() : Impl() {}
        Interruptable_Uint64_Arch(uint64_t value) : Impl(value) {}
        Interruptable_Uint64_Arch(const Interruptable_Uint64_Arch& rhs) : Impl(rhs.load()) {}

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

    template <typename Impl>
    class Interruptable_Int64_Arch : public Impl {
      public:
        Interruptable_Int64_Arch() : Impl() {}
        Interruptable_Int64_Arch(int64_t value) : Impl(value) {}
        Interruptable_Int64_Arch(const Interruptable_Int64_Arch& rhs) : Impl(rhs.load()) {}

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
    /** @} */

} // namespace detail

/**
 * @name Architecture-agnostic interruptable signed/unsigned 64bit integer variable.
 * @note Always use this typedef in your code!
 * @{
 */
using Interruptable_Uint64 = detail::Interruptable_Uint64_Arch<Interruptable_Uint64_Arch_Impl>;
using Interruptable_Int64  = detail::Interruptable_Int64_Arch<Interruptable_Int64_Arch_Impl>;
/** @} */

} // namespace RODOS
