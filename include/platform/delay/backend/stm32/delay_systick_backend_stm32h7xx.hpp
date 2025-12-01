// delay_systick_backend_stm32h7xx.hpp
#pragma once

#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "empp/define.hpp"
    #include "platform/delay/delay_state.hpp"

namespace empp::stm32h7xx::delay {

/**
 * @brief STM32H7 SysTick 的 24-bit 延时 Backend。
 *
 * @details
 * 使用 Cortex-M SysTick 作为简单倒计时定时器，仅用于阻塞式延时，不启用中断。
 * 通过 DelayState::ticks_per_us 记录“每 1us 需要的 CPU 周期数”（= sysclk
 * MHz）。
 *
 * @warning
 * 若 SysTick 已被 RTOS 等用作系统节拍计数，则不应再由此 Backend
 * 直接修改SysTick->LOAD/VAL/CTRL。
 */
struct SysTickBackend
{
    /// 存放 ticks_per_us 的全局状态。
    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t SYSTICK_CLKSOURCE_CPU =
        SysTick_CTRL_CLKSOURCE_Msk;
    static constexpr uint32_t SYSTICK_ENABLE    = SysTick_CTRL_ENABLE_Msk;
    static constexpr uint32_t SYSTICK_COUNTFLAG = SysTick_CTRL_COUNTFLAG_Msk;

    /**
     * @brief 初始化 SysTick。
     *
     * @param sysclk_mhz 系统主频（MHz），如 480 表示 480MHz。
     *
     * @details
     * 这里的操作是：
     * - 选择 SysTick 时钟源为 CPU 时钟；
     * - 先关闭 SysTick；
     * - 设置 DelayState::ticks_per_us = sysclk_mhz。
     */
    EMPP_STATIC_INLINE void init(const uint16_t sysclk_mhz) EMPP_NOEXCEPT
    {
        SysTick->CTRL |=
            SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
        SysTick->CTRL &= ~SYSTICK_ENABLE;

        delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
    }

    /**
     * @brief 阻塞式延时 nUs 微秒。
     *
     * @param nUs 延时时间（单位: us）。
     *
     * @details
     * 根据 ticks_per_us 计算总 tick 数，配置 SysTick->LOAD/VAL 并轮询
     * COUNTFLAG。
     *
     * @warning
     * 上层 DelayImpl 在 DEBUG 下会对 nUs 进行范围断言，防止溢出
     * 24bit 计数， 也就是这里传入的参数范围是(0, 2^24 / ticks_per_us)。
     * 例如在 500MHz 时，单次最大延时约 33,554 us。
     * 请不要传入 0。
     */
    EMPP_STATIC_INLINE void us(const uint32_t nUs) EMPP_NOEXCEPT
    {
        const uint32_t ticks = nUs * delay_state::ticks_per_us;

        SysTick->LOAD = ticks - 1U;      // 加载倒计时时间
        SysTick->VAL  = 0U;              // 清空当前计数
        SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

        // 等待计数完成
        while ((SysTick->CTRL & SYSTICK_COUNTFLAG) == 0U) {
            __NOP();
        }

        SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止SysTick
    }

    /**
     * @brief 阻塞式延时 nMs 毫秒，自动拆分成多段以避免 SysTick 溢出。
     *
     * @param nMs 延时时间（单位: ms）。
     *
     * @details
     * SysTick 为 24bit 计数器，在高主频时单段最大可延时有限；
     * 这里按 max_chunk_ms 一段一段调用 私有成员函数 xms()。
     * 拆分逻辑是 500MHz 时单次最大ms < 33.5ms，这里取 30ms 作为拆分。
     *
     * @warning
     * 请不要传入 0。
     */
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
        const uint32_t ticks =
            static_cast<uint32_t>(nMs) * delay_state::ticks_per_us * 1000U;

        SysTick->LOAD = ticks - 1U;      // 加载倒计时时间
        SysTick->VAL  = 0U;              // 清空当前计数
        SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计时

        // 等待计数完成
        while ((SysTick->CTRL & SYSTICK_COUNTFLAG) == 0U) {
            __NOP();
        }

        SysTick->CTRL &= ~SYSTICK_ENABLE; // 停止 SysTick
    }
};

} // namespace empp::stm32h7xx::delay

#endif
