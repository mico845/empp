// gpio_ports.hpp
#pragma once
#include "empp/driver.hpp"
#include "empp/type.hpp"

namespace empp::platform::gpio {

struct PortA
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOA; }
};

struct PortB
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOB; }
};

struct PortC
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOC; }
};

struct PortD
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOD; }
};

struct PortE
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOE; }
};

struct PortH
{
    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept { return GPIOH; }
};

// More

} // namespace empp::platform::gpio
