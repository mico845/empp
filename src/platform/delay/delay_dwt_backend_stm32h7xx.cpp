#include "platform/delay/backend/stm32/delay_dwt_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::delay {

void DWTBackend::init(const uint16_t sysclk_mhz) noexcept
{
    CoreDebug->DEMCR |= COREDEBUG_DWT_ENABLE; /* 使能DWT外设 */
    DWT->CYCCNT = 0;                          /* DWT CYCCNT 寄存器计数清0 */
    DWT->CTRL |= DWT_ENABLE; /* 使能Cortex-M DWT CYCCNT 寄存器 */

    delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}

} // namespace empp::stm32h7xx::delay
