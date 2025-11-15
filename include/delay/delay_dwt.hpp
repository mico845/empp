#pragma once
#include "empp/type.hpp"
#include "empp/driver.hpp"

namespace empp::platform::delay::dwt {

inline uint32_t ticks_per_us = 0;

constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

void init();
void init(uint16_t sysclk_mhz);

inline void us(const uint32_t nUs)
{
    const uint32_t start = DWT->CYCCNT;
    const uint32_t ticks = nUs * ticks_per_us;
    while ((uint32_t)(DWT->CYCCNT - start) < ticks) {
        __NOP();
    }
}

// 延时 n 毫秒（单次最大约 8589.93ms @500MHz）
inline void ms(uint16_t nMs) { us(static_cast<uint32_t>(nMs * 1000ULL)); }

inline void s(uint16_t nS)
{
    while (nS--)
        ms(1000);
}

} // namespace empp::platform::delay::dwt
