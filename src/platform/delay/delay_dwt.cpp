#include "platform/delay/delay_dwt.hpp"
#include "platform/rcc/rcc.hpp"

namespace empp::platform::delay {

void DWTBackend::init(uint16_t sysclk_mhz) noexcept
{
    CoreDebug->DEMCR |= COREDEBUG_DWT_ENABLE; /* 使能DWT外设 */
    DWT->CYCCNT = 0;                          /* DWT CYCCNT 寄存器计数清0 */
    DWT->CTRL |= DWT_ENABLE; /* 使能Cortex-M DWT CYCCNT 寄存器 */

    ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}

} // namespace empp::platform::delay
