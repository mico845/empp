// delay_if.hpp
#pragma once
#include "delay_impl.hpp"
#include "empp_config.hpp"

namespace empp::stm32h7xx::delay {

#if EMPP_DELAY_USE_DWT
using Backend = platform::delay::DWTBackend;
#elif EMPP_DELAY_USE_SYSTICK
using Backend = platform::delay::SysTickBackend;
#else
using Backend = platform::delay::DWTBackend; // 默认用 DWT
#endif

using Impl = platform::delay::DelayImpl<Backend>;

EMPP_ALWAYS_INLINE void init() noexcept { Impl::init(); }

EMPP_ALWAYS_INLINE void init(const uint32_t sysclk_mhz) noexcept
{
    Impl::init(sysclk_mhz);
}

EMPP_ALWAYS_INLINE void us(const uint32_t nUs) noexcept { Impl::us(nUs); }

EMPP_ALWAYS_INLINE void ms(const uint16_t nMs) noexcept { Impl::ms(nMs); }

EMPP_ALWAYS_INLINE void s(const uint16_t nS) noexcept { Impl::s(nS); }

} // namespace empp::stm32h7xx::delay