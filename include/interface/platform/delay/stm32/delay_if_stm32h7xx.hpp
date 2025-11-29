// delay_if_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/define.hpp"
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

EMPP_STATIC_INLINE void init() EMPP_NOEXCEPT { Impl::init(EMPP_SYSCLK_MHZ); }

EMPP_STATIC_INLINE void init(const unsigned int sysclk_mhz) EMPP_NOEXCEPT
{
    Impl::init(sysclk_mhz);
}

EMPP_STATIC_INLINE void us(const size_t nUs) EMPP_NOEXCEPT { Impl::us(nUs); }

EMPP_STATIC_INLINE void ms(const size_t nMs) EMPP_NOEXCEPT { Impl::ms(nMs); }

EMPP_STATIC_INLINE void s(const size_t nS) EMPP_NOEXCEPT { Impl::s(nS); }

    #if EMPP_DELAY_USE_DWT
/* 测量运行时间 */
EMPP_STATIC_INLINE uint32_t now_cycles() EMPP_NOEXCEPT
{
    return Backend::now_cycles();
}

EMPP_STATIC_INLINE uint32_t elapsed_cycles(const uint32_t start) EMPP_NOEXCEPT
{
    return Backend::elapsed_cycles(start);
}

EMPP_STATIC_INLINE uint32_t cycles_to_us(const uint32_t cycles) EMPP_NOEXCEPT
{
    return Backend::cycles_to_us(cycles);
}

template <typename F>
EMPP_STATIC_INLINE uint32_t measure_cycles(F &&f) EMPP_NOEXCEPT
{
    const volatile uint32_t start = Backend::now_cycles();
    std::forward<F>(f)();
    return Backend::elapsed_cycles(start);
}

template <typename F>
EMPP_STATIC_INLINE uint32_t measure_us(F &&f) EMPP_NOEXCEPT
{
    const volatile uint32_t cycles = measure_cycles(std::forward<F>(f));
    return Backend::cycles_to_us(cycles);
}

    #endif

} // namespace empp::stm32h7xx::delay
#endif