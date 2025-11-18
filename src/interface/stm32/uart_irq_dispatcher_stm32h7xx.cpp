// uart_irq_dispatcher_stm32h7xx.cpp
#include "interface/uart/stm32/uart_irq_dispatcher_stm32h7xx.h"
#include "interface/uart/stm32/uart_if_stm32h7xx.hpp"

using U1 = empp::stm32h7xx::uart::U1;
using U2 = empp::stm32h7xx::uart::U2;
using U3 = empp::stm32h7xx::uart::U3;
using U4 = empp::stm32h7xx::uart::U4;
using U5 = empp::stm32h7xx::uart::U5;
using U6 = empp::stm32h7xx::uart::U6;

extern "C" void EMPP_UART_IRQ_HANDLER(1)(void) { U1::handle_irq(); }
extern "C" void EMPP_UART_IRQ_HANDLER(2)(void) { U2::handle_irq(); }
extern "C" void EMPP_UART_IRQ_HANDLER(3)(void) { U3::handle_irq(); }
extern "C" void EMPP_UART_IRQ_HANDLER(4)(void) { U4::handle_irq(); }
extern "C" void EMPP_UART_IRQ_HANDLER(5)(void) { U5::handle_irq(); }
extern "C" void EMPP_UART_IRQ_HANDLER(6)(void) { U6::handle_irq(); }
