// delay_systick_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"
#if defined(EMPP_CHIP_STM32H7)

    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "empp/define.hpp"
    #include "platform/delay/delay_state.hpp"

namespace empp::stm32h7xx::delay {

struct SysTickBackend
{

    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t SYSTICK_CLKSOURCE_CPU =
        SysTick_CTRL_CLKSOURCE_Msk;
    static constexpr uint32_t SYSTICK_ENABLE    = SysTick_CTRL_ENABLE_Msk;
    static constexpr uint32_t SYSTICK_COUNTFLAG = SysTick_CTRL_COUNTFLAG_Msk;

    EMPP_STATIC_INLINE void init(const uint16_t sysclk_mhz) EMPP_NOEXCEPT
    {
        SysTick->CTRL |=
            SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
        SysTick->CTRL &= ~SYSTICK_ENABLE;

        delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
    }

    EMPP_STATIC_INLINE void us(const uint32_t nUs) EMPP_NOEXCEPT
    {
        if (nUs == 0u || delay_state::ticks_per_us == 0u) {
            return;
        }

        const uint64_t raw_ticks =
            static_cast<uint64_t>(nUs) * delay_state::ticks_per_us;

        auto ticks = static_cast<uint32_t>((raw_ticks > 0x00FFFFFFu)
                                               ? 0x00FFFFFFu
                                               : raw_ticks); // 钳制到 24bit

        if (ticks == 0U) {
            return;
        }

        SysTick->LOAD = ticks - 1U;      // 加载倒计时时间
        SysTick->VAL  = 0U;              // 清空当前计数
        SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

        // 等待计数完成
        while ((SysTick->CTRL & SYSTICK_COUNTFLAG) == 0U) {
            __NOP();
        }

        SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止SysTick
    }

    // 延时 n 毫秒，自动拆分成多段
    EMPP_STATIC_INLINE void ms(const uint16_t nMs) EMPP_NOEXCEPT
    {
        constexpr uint16_t max_chunk_ms =
            30; // 500MHz 时单次 < 33.5ms，这里取 30ms 保险

        uint16_t       repeat = nMs / max_chunk_ms;
        const uint16_t remain = nMs % max_chunk_ms;

        while (repeat-- != 0U) {
            xms(max_chunk_ms);
        }
        if (remain != 0U) {
            xms(remain);
        }
    }

private:
    // 延时 n 毫秒（单次最大约 34.95ms @480MHz）
    EMPP_STATIC_INLINE void xms(const uint16_t nMs) EMPP_NOEXCEPT
    {
        if ((nMs == 0U) || (delay_state::ticks_per_us == 0U)) {
            return;
        }

        const uint64_t raw_ticks =
            static_cast<uint64_t>(nMs) * 1000ULL * delay_state::ticks_per_us;

        auto ticks = static_cast<uint32_t>((raw_ticks > 0x00FFFFFFu)
                                               ? 0x00FFFFFFu
                                               : raw_ticks); // 钳制到 24bit

        if (ticks == 0U) {
            return;
        }

        SysTick->LOAD = ticks - 1U;
        SysTick->VAL  = 0U;              // 清空当前计数
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
