// delay_systick_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"
#if defined(EMPP_CHIP_STM32H7)

    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/delay/delay_state.hpp"

namespace empp::stm32h7xx::delay {

struct SysTickBackend
{

    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t SYSTICK_CLKSOURCE_CPU =
        SysTick_CTRL_CLKSOURCE_Msk;
    static constexpr uint32_t SYSTICK_ENABLE    = SysTick_CTRL_ENABLE_Msk;
    static constexpr uint32_t SYSTICK_COUNTFLAG = SysTick_CTRL_COUNTFLAG_Msk;

    EMPP_ALWAYS_INLINE static void init(const uint16_t sysclk_mhz) noexcept
    {
        SysTick->CTRL |=
            SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
        SysTick->CTRL &= ~SYSTICK_ENABLE;

        delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
    }

    EMPP_ALWAYS_INLINE static void us(const uint32_t nUs) noexcept
    {
        const uint32_t ticks = nUs * delay_state::ticks_per_us;

        SysTick->LOAD = ticks;           // 加载倒计时时间
        SysTick->VAL  = 0;               // 清空当前计数
        SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

        // 等待计数完成
        while (!(SysTick->CTRL & SYSTICK_COUNTFLAG)) {
            __NOP();
        }

        SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止SysTick
    }

    // 延时 n 毫秒，自动拆分成多段
    EMPP_ALWAYS_INLINE static void ms(const uint16_t nMs) noexcept
    {
        constexpr uint16_t max_chunk_ms =
            30; // 500Mhz的时候,delay_xms最大只能延时大约33.5ms，这里使用30

        uint16_t       repeat = nMs / max_chunk_ms;
        const uint16_t remain = nMs % max_chunk_ms;

        while (repeat--) {
            xms(max_chunk_ms);
        }
        if (remain) {
            xms(remain);
        }
    }

private:
    // 延时 n 毫秒（单次最大约 34.95ms @480MHz）
    EMPP_ALWAYS_INLINE static void xms(const uint16_t nMs) noexcept
    {
        const uint32_t ticks = nMs * delay_state::ticks_per_us
                               * 1000u; // 时间加载(SysTick->LOAD为24bit,故 ms
                                        // <= 2^24 *1000/SYSCLK)

        SysTick->LOAD = ticks;           // 加载倒计时时间
        SysTick->VAL  = 0;               // 清空当前计数
        SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

        // 等待计数完成
        while (!(SysTick->CTRL & SYSTICK_COUNTFLAG)) {
            __NOP();
        }

        SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止 SysTick
    }
};

} // namespace empp::stm32h7xx::delay

#endif
