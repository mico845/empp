// delay_dwt_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"
#if defined(EMPP_CHIP_STM32H7)
    #include "empp/assert.hpp"
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "empp/define.hpp"
    #include "platform/delay/delay_state.hpp"

namespace empp::stm32h7xx::delay {

/**
 * @brief STM32H7 DWT CYCCNT 的 32-bit 延时 Backend。
 *
 * @details
 * 使用 Cortex-M7 DWT 单元的 CYCCNT 作为周期计数器：
 * - 支持微秒级延时；
 * - 提供 cycle 级时间测量（now_cycles / elapsed_cycles / cycles_to_us）。
 *
 * @note
 * CYCCNT 为 32 位递增计数器，假设 sysclk <= 500MHz 时，
 * 单次连续计数时间约 8589.93ms。
 */
struct DWTBackend
{
    /// 延时状态，全局保存 ticks_per_us。
    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
    static constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

    /**
     * @brief 初始化 DWT CYCCNT。
     *
     * @param sysclk_mhz 系统主频（MHz）。
     *
     * @details
     * 这里的操作是：
     * - 打开 CoreDebug->DEMCR.TRCENA；
     * - 清零 DWT->CYCCNT；
     * - 使能 DWT->CTRL.CYCCNTENA；
     * - 设置 DelayState::ticks_per_us = sysclk_mhz。
     */
    EMPP_STATIC_INLINE void init(const uint16_t sysclk_mhz) EMPP_NOEXCEPT
    {
        CoreDebug->DEMCR |= COREDEBUG_DWT_ENABLE; /* 使能DWT外设 */
        DWT->CYCCNT = 0U;                         /* DWT CYCCNT 寄存器计数清0 */
        DWT->CTRL |= DWT_ENABLE; /* 使能Cortex-M DWT CYCCNT 寄存器 */

        delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
    }

    /**
     * @brief 阻塞式延时 nUs 微秒。
     *
     * @param nUs 延时时间（单位: us）。
     *
     * @details
     * - 读取起始 cycle；
     * - 计算目标 tick 数：ticks = nUs * ticks_per_us；
     * - 自旋等待 elapsed_cycles >= ticks。
     *
     * @warning
     * 上层 DelayImpl 在 DEBUG 下限制 nUs，避免 nUs 溢出 32bit。
     * 32bit 计数， 也就是这里传入的参数范围是(0, 2^32 / ticks_per_us)。
     * 例如在 500MHz 时，单次最大延时约 8,589,934 us。
     * 请不要传入 0。
     */
    EMPP_STATIC_INLINE void us(const uint32_t nUs) EMPP_NOEXCEPT
    {
        const uint32_t start = DWT->CYCCNT;
        const uint32_t ticks = nUs * delay_state::ticks_per_us;

        while (DWT->CYCCNT - start < ticks) {
            __NOP();
        }
    }

    /**
     * @brief 阻塞式延时 nMs 毫秒。
     *
     * @param nMs 延时时间（单位: ms）。
     *
     * @details
     * 将毫秒转换为微秒 total_us = nMs * 1000，再调用 us()。
     *
     * @warning
     * 上层 DelayImpl 在 DEBUG 下限制 nMs，避免 total_us 溢出 32bit。
     * 32bit 计数， 也就是这里传入的参数范围是(0, 2^32 / ticks_per_us / 1000)。
     * 例如在 500MHz 时，单次最大延时约 8,589 ms。
     */
    EMPP_STATIC_INLINE void ms(const uint16_t nMs) EMPP_NOEXCEPT
    {
        const uint32_t total_us = static_cast<uint32_t>(nMs) * 1000U;
        us(total_us);
    }

    /**
     * @brief 返回当前 cycle 计数值。
     *
     * @return 当前 DWT->CYCCNT。
     */
    EMPP_STATIC_INLINE uint32_t now_cycles() EMPP_NOEXCEPT
    {
        return DWT->CYCCNT;
    }

    /**
     * @brief 计算从 start 到当前的 cycle 差值。
     *
     * @param start 起始 cycle 值，一般由 now_cycles() 获得。
     * @return 经过的 cycles 数（自动处理 32bit 回绕）。
     */
    EMPP_STATIC_INLINE uint32_t elapsed_cycles(const uint32_t start)
        EMPP_NOEXCEPT
    {
        return DWT->CYCCNT - start;
    }

    /**
     * @brief 将 cycle 数转换为微秒（整数近似，向下取整）。
     *
     * @param cycles 周期数。
     * @return 对应的时间（单位: us）。
     *
     * @details
     * 计算公式：us = cycles / ticks_per_us。
     * 调用前需保证 init() 已经设置 ticks_per_us。
     */
    EMPP_STATIC_INLINE uint32_t cycles_to_us(const uint32_t cycles)
        EMPP_NOEXCEPT
    {
        const uint32_t ticks_per_us = delay_state::ticks_per_us;

        EMPP_ASSERT(ticks_per_us > 0U,
                    "DWTBackend::cycles_to_us ticks_per_us is 0");

        return cycles / ticks_per_us;
    }
};

} // namespace empp::stm32h7xx::delay

#endif
