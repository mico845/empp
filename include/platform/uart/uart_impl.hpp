// uart_impl.hpp
#pragma once
#include "uart_id.hpp"

namespace empp::platform::uart {

template <typename UartId>
struct UartImpl
{
    EMPP_ALWAYS_INLINE static void write(uint8_t value) noexcept
    {
        while (!LL_USART_IsActiveFlag_TXE(UartId::regs())) {
        }
        LL_USART_TransmitData8(UartId::regs(), value);
    }
};
} // namespace empp::platform::uart