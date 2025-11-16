// rcc_if_stm32h7xx.hpp
#pragma once
#include "empp/driver.hpp"
#include "empp/type.hpp"
#include "platform/rcc/rcc.hpp"

namespace empp::stm32h7xx::rcc {
EMPP_ALWAYS_INLINE uint32_t EMPP_RCC_GetSystemClockFreq()
{
    return platform::rcc::EMPP_RCC_GetSystemClockFreq();
}
} // namespace empp::stm32h7xx::rcc