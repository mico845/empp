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

struct DWTBackend
{
    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
    static constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

    EMPP_STATIC_INLINE void init(const uint16_t sysclk_mhz) EMPP_NOEXCEPT
    {
        CoreDebug->DEMCR |= COREDEBUG_DWT_ENABLE; /* 使能DWT外设 */
        DWT->CYCCNT = 0U;                         /* DWT CYCCNT 寄存器计数清0 */
        DWT->CTRL |= DWT_ENABLE; /* 使能Cortex-M DWT CYCCNT 寄存器 */

        delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
    }

    EMPP_STATIC_INLINE void us(const uint32_t nUs) EMPP_NOEXCEPT
    {
        const uint32_t start = DWT->CYCCNT;
        const auto     ticks = nUs * delay_state::ticks_per_us;

        while (DWT->CYCCNT - start < ticks) {
            __NOP();
        }
    }

    // 延时 n 毫秒（单次最大约 8589.93ms @500MHz）
    EMPP_STATIC_INLINE void ms(const uint16_t nMs) EMPP_NOEXCEPT
    {
        const auto total_us = nMs * 1000ULL;
        us(total_us);
    }

    EMPP_STATIC_INLINE uint32_t now_cycles() EMPP_NOEXCEPT
    {
        return DWT->CYCCNT;
    }

    EMPP_STATIC_INLINE uint32_t elapsed_cycles(const uint32_t start)
        EMPP_NOEXCEPT
    {
        return DWT->CYCCNT - start;
    }

    // 把 cycles 转换为 us（整数近似）
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
