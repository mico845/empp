// delay_impl.hpp
#pragma once
#include "delay_concept.hpp"
#include "delay_dwt.hpp"
#include "delay_systick.hpp"

namespace empp::platform::delay {

template <DelayBackend Backend>
struct DelayImpl
{
    static void init() noexcept { Backend::init(); }

    static void init(uint16_t sysclk_mhz) noexcept
    {
        Backend::init(sysclk_mhz);
    }

    static void us(uint32_t nUs) noexcept { Backend::us(nUs); }

    static void ms(uint16_t nMs) noexcept { Backend::ms(nMs); }

    static void s(uint16_t nS) noexcept { Backend::s(nS); }
};

using DelayDwt     = DelayImpl<DWTBackend>;
using DelaySysTick = DelayImpl<SysTickBackend>;

} // namespace empp::platform::delay
