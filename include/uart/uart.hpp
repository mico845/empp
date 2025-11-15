#pragma once
#include "uart_id.hpp"

namespace empp::platform::uart {

template <typename UartId>
struct Uart
{

    static void write() { UartId::regs(); }
};
} // namespace empp::platform::uart