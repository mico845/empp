#pragma once
#include "empp/type.hpp"
#include "empp/driver.hpp"

namespace empp::platform::delay::systick {

inline volatile uint32_t ticks_per_us = 0;

constexpr uint32_t SYSTICK_CLKSOURCE_CPU = SysTick_CTRL_CLKSOURCE_Msk;
constexpr uint32_t SYSTICK_ENABLE        = SysTick_CTRL_ENABLE_Msk;
constexpr uint32_t SYSTICK_COUNTFLAG     = SysTick_CTRL_COUNTFLAG_Msk;

void init();
void init(uint16_t sysclk_mhz);

inline void us(const uint32_t nUs)
{
    const uint32_t ticks = nUs * ticks_per_us;

    SysTick->LOAD = ticks;           // 加载倒计时时间
    SysTick->VAL  = 0;               // 清空当前计数
    SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

    // 等待计数完成
    while (!(SysTick->CTRL & SYSTICK_COUNTFLAG)) {
    }

    SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止SysTick
}

// 延时 n 毫秒（单次最大约 34.95ms @480MHz）
inline void xms(const uint16_t nMs)
{
    const uint32_t ticks =
        nMs * ticks_per_us
        * 1000u; // 时间加载(SysTick->LOAD为24bit,故 ms <= 2^24 *1000/SYSCLK)

    SysTick->LOAD = ticks;           // 加载倒计时时间
    SysTick->VAL  = 0;               // 清空当前计数
    SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

    // 等待计数完成
    while (!(SysTick->CTRL & SYSTICK_COUNTFLAG)) {
    }

    SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止 SysTick
}

// 延时 n 毫秒，自动拆分成多段
inline void ms(const uint16_t nMs)
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

} // namespace empp::platform::delay::systick
