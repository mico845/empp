// uart_if_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/uart/uart_impl.hpp"
    #include "platform/uart/uart_io.hpp"
    #include "platform/uart/backend/stm32/uart_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::uart {

template <uint8_t UartId, typename DmaTx = void, typename DmaRx = void>
using Uart = platform::uart::UartIO<UARTBackend<UartId, DmaTx, DmaRx>>;

using U1 = Uart<1>;
using U2 = Uart<2>;
using U3 = Uart<3>;
using U4 = Uart<4>;
using U5 = Uart<5>;
using U6 = Uart<6>;

} // namespace empp::stm32h7xx::uart

#endif
