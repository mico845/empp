#include "delay/delay_dwt.hpp"
#include "rcc/rcc.hpp"

namespace empp::platform::delay::dwt {

void init()
{
    const uint32_t sysclk_mhz = rcc::EMPP_RCC_GetSystemClockFreq() / 1'000'000;
    init(static_cast<uint16_t>(sysclk_mhz));
}
void init(uint16_t sysclk_mhz)
{
    CoreDebug->DEMCR |= COREDEBUG_DWT_ENABLE; /* 使能DWT外设 */
    DWT->CYCCNT = 0;                          /* DWT CYCCNT寄存器计数清0 */
    DWT->CTRL |= DWT_ENABLE; /* 使能Cortex-M DWT CYCCNT寄存器 */

    ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}

} // namespace empp::platform::delay::dwt
