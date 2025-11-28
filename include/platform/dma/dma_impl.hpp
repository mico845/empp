// dma_impl.hpp
#pragma once
#include "dma_concept.hpp"
#include "empp/type.hpp"

namespace empp::platform::dma {

template <DmaBackend Backend>
struct DmaImpl
{
    EMPP_ALWAYS_INLINE static void configAddr(const uint32_t peripheralAddr,
                                              const uint32_t memoryAddr,
                                              const uint16_t length) noexcept
    {
        Backend::configAddr(peripheralAddr, memoryAddr, length);
    }
    EMPP_ALWAYS_INLINE static void enable() noexcept { Backend::enable(); }

    EMPP_ALWAYS_INLINE static void disable() noexcept { Backend::disable(); }

    EMPP_ALWAYS_INLINE static void clear_all_flags() noexcept
    {
        Backend::clear_all_flags();
    }

    EMPP_ALWAYS_INLINE static uint16_t get_length() noexcept
    {
        return Backend::get_length();
    }

    EMPP_ALWAYS_INLINE static bool is_tc() noexcept { return Backend::is_tc(); }

    EMPP_ALWAYS_INLINE static void clear_tc() noexcept { Backend::clear_tc(); }

    EMPP_ALWAYS_INLINE static bool is_ht() noexcept { return Backend::is_ht(); }

    EMPP_ALWAYS_INLINE static void clear_ht() noexcept { Backend::clear_ht(); }

    EMPP_ALWAYS_INLINE static void enable_irq_tc() noexcept
    {
        Backend::enable_irq_tc();
    }

    EMPP_ALWAYS_INLINE static void disable_irq_tc() noexcept
    {
        Backend::disable_irq_tc();
    }

    EMPP_ALWAYS_INLINE static void enable_irq_ht() noexcept
    {
        Backend::enable_irq_ht();
    }

    EMPP_ALWAYS_INLINE static void disable_irq_ht() noexcept
    {
        Backend::disable_irq_ht();
    }
};

} // namespace empp::platform::dma