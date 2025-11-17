// delay_dwt_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.hpp"
#if defined(EMPP_CHIP_STM32H7)

    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/delay/delay_state.hpp"

namespace empp::stm32h7xx::delay {

struct DWTBackend
{
    using delay_state = platform::delay::DelayState;

    static constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
    static constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

    static void init(uint16_t sysclk_mhz) noexcept;

    EMPP_ALWAYS_INLINE static void us(const uint32_t nUs) noexcept
    {
        const uint32_t start = DWT->CYCCNT;
        const uint32_t ticks = nUs * delay_state::ticks_per_us;
        while (DWT->CYCCNT - start < ticks) {
            __NOP();
        }
    }

    // 延时 n 毫秒（单次最大约 8589.93ms @500MHz）
    EMPP_ALWAYS_INLINE static void ms(const uint16_t nMs) noexcept
    {
        us(static_cast<uint32_t>(nMs * 1000ULL));
    }
};

} // namespace empp::stm32h7xx::delay

#endif
