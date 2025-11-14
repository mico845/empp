#pragma once
#include "empp/driver.hpp"

namespace empp::stm32h7::gpio {

struct PortA
{
    static GPIO_TypeDef *regs() { return GPIOA; }
};

struct PortB
{
    static GPIO_TypeDef *regs() { return GPIOB; }
};

struct PortC
{
    static GPIO_TypeDef *regs() { return GPIOC; }
};

// More

} // namespace empp::stm32h7::gpio
