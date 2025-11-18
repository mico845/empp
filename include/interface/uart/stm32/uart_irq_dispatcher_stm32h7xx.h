// uart_irq_dispatcher_stm32h7xx.hpp
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define EMPP_UART_IRQ_HANDLER(ID) empp_uart##ID##_irq_handler

void EMPP_UART_IRQ_HANDLER(1)(void);
void EMPP_UART_IRQ_HANDLER(2)(void);
void EMPP_UART_IRQ_HANDLER(3)(void);
void EMPP_UART_IRQ_HANDLER(4)(void);
void EMPP_UART_IRQ_HANDLER(5)(void);
void EMPP_UART_IRQ_HANDLER(6)(void);

#ifdef __cplusplus
} // extern "C"
#endif
