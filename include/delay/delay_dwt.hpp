#pragma once
#include "empp/type.hpp"
#include "empp/driver.hpp"

namespace empp::platform::delay::dwt {

inline volatile uint32_t ticks_per_us = 0;

constexpr uint32_t COREDEBUG_DWT_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
constexpr uint32_t DWT_ENABLE           = DWT_CTRL_CYCCNTENA_Msk;

void init();
void init(uint16_t sysclk_mhz);

inline void us(const uint32_t nUs)
{
    const uint32_t start = DWT->CYCCNT;
    const uint32_t ticks = nUs * ticks_per_us;
    while ((uint32_t)(DWT->CYCCNT - start) < ticks) {
    }
}

inline void xms(uint16_t nMs)
{
    while (nMs--)
        us(1000); // 500Mhz的时候,delay_xms最大只能延时大约33.5ms，这里使用30
}

inline void ms(const uint16_t nMs) { xms(nMs); }

} // namespace empp::platform::delay::dwt
