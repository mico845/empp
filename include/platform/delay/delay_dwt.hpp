// delay_dwt.hpp
#pragma once
#include "empp/driver.hpp"
#include "empp/type.hpp"

namespace empp::platform::delay {

struct DWTBackend
{
    static inline uint32_t ticks_per_us =
        0; // 每 1us 需要多少个 CPU 周期（= sysclk MHz）

    static constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
    static constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

    static void init(uint16_t sysclk_mhz) noexcept;

    EMPP_ALWAYS_INLINE static void us(const uint32_t nUs) noexcept
    {
        const uint32_t start = DWT->CYCCNT;
        const uint32_t ticks = nUs * ticks_per_us;
        while (DWT->CYCCNT - start < ticks) {
            __NOP();
        }
    }

    // 延时 n 毫秒（单次最大约 8589.93ms @500MHz）
    EMPP_ALWAYS_INLINE static void ms(const uint16_t nMs) noexcept
    {
        us(static_cast<uint32_t>(nMs * 1000ULL));
    }

    EMPP_ALWAYS_INLINE static void s(uint16_t nS) noexcept
    {
        while (nS--)
            ms(1000);
    }
};

} // namespace empp::platform::delay
