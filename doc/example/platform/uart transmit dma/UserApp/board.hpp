// board.hpp
#pragma once

#include <empp.h>

namespace board {

using namespace empp::stm32h7xx;
using namespace empp::components;

using Uart1RxDma = void;
using Uart1TxDma = dma::Dma2S7;
using Com1       = uart::UartDma<1, Uart1TxDma, Uart1RxDma>;
using Led        = gpio::PC13;
} // namespace board