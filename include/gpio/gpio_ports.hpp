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

struct PortD
{
    static GPIO_TypeDef *regs() { return GPIOD; }
};

struct PortE
{
    static GPIO_TypeDef *regs() { return GPIOE; }
};

struct PortH
{
    static GPIO_TypeDef *regs() { return GPIOH; }
};

// More

} // namespace empp::stm32h7::gpio
