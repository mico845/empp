// delay_impl.hpp
#pragma once
#include "empp_config.h"
#include "empp/assert.hpp"
#include "empp/define.hpp"
#include "delay_concept.hpp"
#include "platform/delay/delay_state.hpp"

namespace empp::platform::delay {

template <DelayBackend Backend>
struct DelayImpl
{
    EMPP_STATIC_INLINE void init(unsigned int sysclk_mhz) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        EMPP_ASSERT(
            sysclk_mhz <= 1000U,
            "STM32H7 sysclk_mhz must be <= 1000 MHz"); /* H7 暂时不会超频到
                                                          1 GHZ */
#endif
        Backend::init(sysclk_mhz);
    }

    EMPP_STATIC_INLINE void us(size_t nUs) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(nUs > 0U, "us called with 0 us");
        EMPP_ASSERT(platform::delay::DelayState::ticks_per_us > 0U,
                    "ticks_per_us is 0 (maybe delay not initialized?)");
    #if defined(EMPP_CHIP_STM32H7)

        #if (EMPP_DELAY_USE_DWT == 1U)
        // DWT 路径：32bit CYCCNT，假设 sysclk <= 500 MHz
        // 最大单次 us: floor(2^32 / 500) ≈ 8,589,934 us (0x83126E)
        EMPP_ASSERT(nUs <= 0x83126EU,
                    "STM32H7 DWT CYCCNT overflow 32-bit in us()");
        #elif (EMPP_DELAY_USE_SYSTICK == 1U)
        // SysTick 路径：24bit 向下计数器，LOAD ∈ [1, 0x00FFFFFF]
        // 假设 sysclk = 500 MHz，则最大 us: floor(2^24 / 500) ≈ 33,554 us
        EMPP_ASSERT(nUs <= 33554U,
                    "STM32H7 SysTick LOAD exceeds 24-bit in us()");
        #endif

    #endif
#endif

        Backend::us(nUs);
    }

    EMPP_STATIC_INLINE void ms(size_t nMs) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(nMs > 0U, "ms called with 0 ms");
        EMPP_ASSERT(platform::delay::DelayState::ticks_per_us > 0U,
                    "ticks_per_us is 0 (maybe delay not initialized?)");
    #if defined(EMPP_CHIP_STM32H7)
        #if (EMPP_DELAY_USE_DWT == 1U)
        // DWT 路径：最大约 8,589,934 us @500MHz，对应约 8589 ms
        EMPP_ASSERT(nMs <= 8589U, "STM32H7 DWT CYCCNT overflow 32-bit in ms()");
        #endif
    #endif

#endif

        Backend::ms(nMs);
    }

    EMPP_STATIC_INLINE void s(size_t nS) EMPP_NOEXCEPT
    {
        while (nS--)
            ms(1000);
    }
};

} // namespace empp::platform::delay
