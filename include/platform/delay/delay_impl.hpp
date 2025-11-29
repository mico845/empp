// delay_impl.hpp
#pragma once

#include "empp/define.hpp"
#include "delay_concept.hpp"

namespace empp::platform::delay {

template <DelayBackend Backend>
struct DelayImpl
{
    EMPP_STATIC_INLINE void init(uint16_t sysclk_mhz) EMPP_NOEXCEPT 
    {
        Backend::init(sysclk_mhz);
    }

    EMPP_STATIC_INLINE void us(uint32_t nUs) EMPP_NOEXCEPT  { Backend::us(nUs); }

    EMPP_STATIC_INLINE void ms(uint16_t nMs) EMPP_NOEXCEPT  { Backend::ms(nMs); }

    EMPP_STATIC_INLINE void s(uint16_t nS) EMPP_NOEXCEPT 
    {
        while (nS--)
            ms(1000);
    }
};

} // namespace empp::platform::delay
