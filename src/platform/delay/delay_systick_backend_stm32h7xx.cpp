#include "platform/delay/backend/stm32/delay_systick_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::delay {

void SysTickBackend::init(const uint16_t sysclk_mhz) noexcept
{
    SysTick->CTRL |= SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
    SysTick->CTRL &= ~SYSTICK_ENABLE;

    delay_state::ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}
} // namespace empp::stm32h7xx::delay
