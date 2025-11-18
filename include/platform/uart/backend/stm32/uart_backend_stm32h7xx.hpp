// uart_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.hpp"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/uart/uart_dispatcher.hpp"

namespace empp::stm32h7xx::uart {

using Irq = platform::uart::UartDispatcher;

template <uint8_t UartId>
struct UARTBackend
{
    EMPP_ALWAYS_INLINE static USART_TypeDef *regs() noexcept
    {
        if constexpr (UartId == 1)
            return USART1;
        if constexpr (UartId == 2)
            return USART2;
        if constexpr (UartId == 3)
            return USART3;
        if constexpr (UartId == 4)
            return UART4;
        if constexpr (UartId == 5)
            return UART5;
        if constexpr (UartId == 6)
            return USART6;
        return USART1;
    }

    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        while (!(regs()->ISR & USART_ISR_TXE_TXFNF)) {
        }
        regs()->TDR = value;
    }

    EMPP_ALWAYS_INLINE static void enable_tx_irq() noexcept
    {
        regs()->CR1 |= USART_CR1_TXEIE;
    }

    EMPP_ALWAYS_INLINE static void disable_tx_irq() noexcept
    {
        regs()->CR1 &= ~USART_CR1_TXEIE;
    }

    EMPP_ALWAYS_INLINE static void
    register_callback_tx(const Callback cb) noexcept
    {
        Irq::register_callback_tx(UartId, cb);
    }

    EMPP_ALWAYS_INLINE static void handle_irq() noexcept
    {
        if (regs()->ISR & USART_ISR_TXE_TXFNF) {
            Irq::dispatch_tx(UartId);
        }
    }
};

} // namespace empp::stm32h7xx::uart

#endif