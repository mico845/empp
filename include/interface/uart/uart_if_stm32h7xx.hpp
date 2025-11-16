// uart_if_stm32h7xx.hpp
#pragma once
#include "platform/uart/uart_io.hpp"

namespace empp::stm32h7xx::uart {

using U1 = platform::uart::UartIO<platform::uart::Uart1>;
using U2 = platform::uart::UartIO<platform::uart::Uart2>;
using U3 = platform::uart::UartIO<platform::uart::Uart3>;

} // namespace empp::stm32h7xx::uart