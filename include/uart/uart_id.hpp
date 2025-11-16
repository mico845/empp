// uart_id.hpp
#pragma once
#include "empp/driver.hpp"

namespace empp::platform::uart {

struct Uart1
{
    static USART_TypeDef *regs() noexcept { return USART1; }
};
struct Uart2
{
    static USART_TypeDef *regs() noexcept { return USART2; }
};
struct Uart3
{
    static USART_TypeDef *regs() noexcept { return USART3; }
};

/* More */

} // namespace empp::platform::uart