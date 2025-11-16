#pragma once
#include "uart_id.hpp"

namespace empp::platform::uart {

template <typename UartId>
struct Uart
{
    static void write(uint8_t value)
    {
        while (!LL_USART_IsActiveFlag_TXE(UartId::regs())) {
        }
        LL_USART_TransmitData8(UartId::regs(), value);
    }
};
} // namespace empp::platform::uart