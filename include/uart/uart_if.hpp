#pragma once
#include "uart_io.hpp"

namespace empp::stm32h7xx::uart {

using Uart1 = platform::uart::UartIO<platform::uart::Uart1>;
using Uart2 = platform::uart::UartIO<platform::uart::Uart2>;
using Uart3 = platform::uart::UartIO<platform::uart::Uart3>;

} // namespace empp::stm32h7xx::uart