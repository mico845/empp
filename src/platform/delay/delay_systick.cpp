#include "platform/delay/delay_systick.hpp"
#include "platform/rcc/rcc.hpp"

namespace empp::platform::delay {

void SysTickBackend::init(const uint16_t sysclk_mhz) noexcept
{
    SysTick->CTRL |= SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
    SysTick->CTRL &= ~SYSTICK_ENABLE;

    ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}
} // namespace empp::platform::delay
