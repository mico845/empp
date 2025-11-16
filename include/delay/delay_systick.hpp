// delay_systick.hpp
#pragma once
#include "empp/driver.hpp"
#include "empp/type.hpp"

namespace empp::platform::delay {

struct SysTickBackend
{
    static inline uint32_t ticks_per_us = 0;

    static constexpr uint32_t SYSTICK_CLKSOURCE_CPU =
        SysTick_CTRL_CLKSOURCE_Msk;
    static constexpr uint32_t SYSTICK_ENABLE    = SysTick_CTRL_ENABLE_Msk;
    static constexpr uint32_t SYSTICK_COUNTFLAG = SysTick_CTRL_COUNTFLAG_Msk;

    static void init() noexcept;
    static void init(uint16_t sysclk_mhz) noexcept;

    EMPP_ALWAYS_INLINE static void us(const uint32_t nUs) noexcept
    {
        const uint32_t ticks = nUs * ticks_per_us;

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

    EMPP_ALWAYS_INLINE static void s(uint16_t nS) noexcept
    {
        while (nS--)
            ms(1000);
    }

private:
    // 延时 n 毫秒（单次最大约 34.95ms @480MHz）
    EMPP_ALWAYS_INLINE static void xms(const uint16_t nMs) noexcept
    {
        const uint32_t ticks =
            nMs * ticks_per_us * 1000u; // 时间加载(SysTick->LOAD为24bit,故 ms
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

} // namespace empp::platform::delay
