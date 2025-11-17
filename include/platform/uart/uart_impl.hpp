// uart_impl.hpp
#pragma once
#include "uart_id.hpp"

namespace empp::platform::uart {

template <typename UartId>
struct UartImpl
{
    EMPP_ALWAYS_INLINE static void write(uint8_t value) noexcept
    {
        while (!(UartId::regs()->ISR & USART_ISR_TXE_TXFNF)) {
        }
        UartId::regs()->TDR = value;
    }
};
} // namespace empp::platform::uart