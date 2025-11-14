#include "delay/delay_systick.hpp"
#include "rcc/rcc.hpp"

namespace empp::platform::delay::systick {

void init()
{
    const uint32_t sysclk_mhz = rcc::EMPP_RCC_GetSystemClockFreq() / 1'000'000;
    init(static_cast<uint16_t>(sysclk_mhz));
}

void init(const uint16_t sysclk_mhz)
{
    SysTick->CTRL |= SYSTICK_CLKSOURCE_CPU; //  clock source -> Processor clock
    SysTick->CTRL &= ~SYSTICK_ENABLE;

    ticks_per_us = sysclk_mhz; // 1us = sysclk_mhz tick
}
} // namespace empp::platform::delay::systick
