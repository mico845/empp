#pragma once
#include "empp/driver.hpp"

namespace empp::platform::uart {

struct Uart1
{
    static USART_TypeDef *regs() { return USART1; }
};

/* More */

} // namespace empp::platform::uart