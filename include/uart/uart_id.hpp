#pragma once
#include "empp/driver.hpp"

namespace empp::stm32h7::uart {

struct Uart1
{
    static USART_TypeDef *regs() { return USART1; }
};

} // namespace empp::stm32h7::uart