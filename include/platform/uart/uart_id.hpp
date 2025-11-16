// uart_id.hpp
#pragma once
#include "empp/driver.hpp"
#include "empp/type.hpp"

namespace empp::platform::uart {

struct Uart1
{
    EMPP_ALWAYS_INLINE static USART_TypeDef *regs() noexcept { return USART1; }
};
struct Uart2
{
    EMPP_ALWAYS_INLINE static USART_TypeDef *regs() noexcept { return USART2; }
};
struct Uart3
{
    EMPP_ALWAYS_INLINE static USART_TypeDef *regs() noexcept { return USART3; }
};

/* More */

} // namespace empp::platform::uart