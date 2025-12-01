// board.hpp
#pragma once

#include "empp.h"

namespace board {

using namespace empp::stm32h7xx;
using namespace empp::components;

/*
 * Uart1 TX DMA (DMA2 Stream7)
 * Mode: Normal
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1TxDma = dma::Dma2S7;
using Uart1RxDma = void;

using Com1 = uart::Uart<1, Uart1TxDma, Uart1RxDma>;
using Led  = gpio::PC13;
} // namespace board