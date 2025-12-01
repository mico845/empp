// board.hpp
#pragma once

#include "empp.h"

namespace board {

using namespace empp::stm32h7xx;
using namespace empp::components;

/*
 * Uart1 RX DMA (DMA2 Stream6)
 * Mode: Normal
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1RxDma = dma::Dma2S6;

using Com1 = uart::Uart<1, void, Uart1RxDma>;
using Led  = gpio::PC13;
} // namespace board