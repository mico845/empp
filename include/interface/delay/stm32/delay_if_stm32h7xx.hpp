// delay_if_stm32h7xx.hpp
#pragma once
#include "empp_config.hpp"

#if defined(EMPP_CHIP_STM32H7)
    #include "platform/delay/delay_impl.hpp"
    #include "platform/delay/backend/stm32/delay_dwt_backend_stm32h7xx.hpp"
    #include "platform/delay/backend/stm32/delay_systick_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::delay {

    #if EMPP_DELAY_USE_DWT
using Backend = DWTBackend;
    #elif EMPP_DELAY_USE_SYSTICK
using Backend = SysTickBackend;
    #else
using Backend = DWTBackend; // 默认用 DWT
    #endif

using Impl = platform::delay::DelayImpl<Backend>;

EMPP_ALWAYS_INLINE void init() noexcept { Impl::init(EMPP_SYSCLK_MHZ); }

EMPP_ALWAYS_INLINE void init(const uint32_t sysclk_mhz) noexcept
{
    Impl::init(sysclk_mhz);
}

EMPP_ALWAYS_INLINE void us(const uint32_t nUs) noexcept { Impl::us(nUs); }

EMPP_ALWAYS_INLINE void ms(const uint16_t nMs) noexcept { Impl::ms(nMs); }

EMPP_ALWAYS_INLINE void s(const uint16_t nS) noexcept { Impl::s(nS); }

} // namespace empp::stm32h7xx::delay
#endif