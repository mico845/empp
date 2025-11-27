// uart_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/uart/uart_dispatcher.hpp"

namespace empp::stm32h7xx::uart {

using Irq = platform::uart::UartDispatcher;

template <uint8_t UartId, typename DmaTx = void, typename DmaRx = void>
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
            /* busy wait */
        }
        regs()->TDR = value;
    }

    EMPP_ALWAYS_INLINE static uint8_t read() noexcept
    {
        while (!(regs()->ISR & USART_ISR_RXNE_RXFNE)) {
            /* busy wait */
        }
        return static_cast<uint8_t>(regs()->RDR);
    }

    EMPP_ALWAYS_INLINE static void enable_irq_tx() noexcept
    {
        regs()->CR1 |= USART_CR1_TXEIE;
    }

    EMPP_ALWAYS_INLINE static void disable_irq_tx() noexcept
    {
        regs()->CR1 &= ~USART_CR1_TXEIE;
    }

    EMPP_ALWAYS_INLINE static void
    register_callback_tx(const Callback cb) noexcept
    {
        Irq::register_callback_tx(UartId, cb);
    }

    EMPP_ALWAYS_INLINE static void enable_irq_rx() noexcept
    {
        regs()->CR1 |= USART_CR1_RXNEIE;
    }

    EMPP_ALWAYS_INLINE static void disable_irq_rx() noexcept
    {
        regs()->CR1 &= ~USART_CR1_RXNEIE;
    }

    EMPP_ALWAYS_INLINE static void
    register_callback_rx(const Callback cb) noexcept
    {
        Irq::register_callback_rx(UartId, cb);
    }

    EMPP_ALWAYS_INLINE static void handle_irq() noexcept
    {
        if (regs()->ISR & USART_ISR_TXE_TXFNF) {
            Irq::dispatch_tx(UartId);
        }

        if (regs()->ISR & USART_ISR_RXNE_RXFNE) {
            Irq::dispatch_rx(UartId);
        }
    }

    EMPP_ALWAYS_INLINE static void enable_dma_rx() noexcept
    {
        regs()->CR3 |= USART_CR3_DMAR;
    }

    EMPP_ALWAYS_INLINE static void disable_dma_rx() noexcept
    {
        regs()->CR3 &= ~USART_CR3_DMAR;
    }

    EMPP_ALWAYS_INLINE static void enable_dma_tx() noexcept
    {
        regs()->CR3 |= USART_CR3_DMAT;
    }

    EMPP_ALWAYS_INLINE static void disable_dma_tx() noexcept
    {
        regs()->CR3 &= ~USART_CR3_DMAT;
    }

    EMPP_ALWAYS_INLINE static void enable_irq_dma_tx_tc() noexcept
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::enable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_ALWAYS_INLINE static void disable_irq_dma_tx_tc() noexcept
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_ALWAYS_INLINE static void config_dma_tx(const uint32_t buffer,
                                                 const uint32_t length) noexcept
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
            DmaTx::clear_all_flags();

            DmaTx::configAddr(reinterpret_cast<uint32_t>(&regs()->TDR), buffer,
                              length);
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /*
     * 每次发生 TXE（使能 FIFO模式时为TXFNF）事件后，
     * 数据都从存储器移动到 USART_TDR。
     */
    EMPP_ALWAYS_INLINE static void start_dma_tx() noexcept
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            regs()->ICR |= USART_ICR_TCCF; // UART TC 标志清零
            DmaTx::enable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_ALWAYS_INLINE static void stop_dma_tx() noexcept
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }
};

} // namespace empp::stm32h7xx::uart

#endif
