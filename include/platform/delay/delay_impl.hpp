// delay_impl.hpp
#pragma once
#include "delay_concept.hpp"

namespace empp::platform::delay {

template <DelayBackend Backend>
struct DelayImpl
{
    EMPP_ALWAYS_INLINE static void init(uint16_t sysclk_mhz) noexcept
    {
        Backend::init(sysclk_mhz);
    }

    EMPP_ALWAYS_INLINE static void us(uint32_t nUs) noexcept
    {
        Backend::us(nUs);
    }

    EMPP_ALWAYS_INLINE static void ms(uint16_t nMs) noexcept
    {
        Backend::ms(nMs);
    }

    EMPP_ALWAYS_INLINE static void s(uint16_t nS) noexcept
    {
        while (nS--)
            ms(1000);
    }
};

} // namespace empp::platform::delay
