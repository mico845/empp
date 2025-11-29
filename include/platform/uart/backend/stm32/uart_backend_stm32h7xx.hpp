// uart_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/dma/dma_concept.hpp"

namespace empp::stm32h7xx::uart {

template <uint8_t UartId, typename DmaTx = void, typename DmaRx = void>
struct UARTBackend
{
    static_assert(std::is_same_v<DmaTx, void>
                      || empp::platform::dma::DmaBackend<DmaTx>,
                  "DmaTx must satisfy empp::platform::dma::DmaBackend");
    static_assert(std::is_same_v<DmaRx, void>
                      || empp::platform::dma::DmaBackend<DmaRx>,
                  "DmaRx must satisfy empp::platform::dma::DmaBackend");

    EMPP_STATIC_INLINE constexpr USART_TypeDef *regs() EMPP_NOEXCEPT
    {
        static_assert(UartId >= 1 && UartId <= 6,
                      "Invalid UartId for STM32H7 UARTBackend");

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

        __builtin_unreachable();
    }

    EMPP_STATIC_INLINE void write(const uint8_t value) EMPP_NOEXCEPT
    {
        while (!(regs()->ISR & USART_ISR_TXE_TXFNF)) {
            /* busy wait */
        }
        regs()->TDR = value;
    }

    EMPP_STATIC_INLINE uint8_t read() EMPP_NOEXCEPT
    {
        while (!(regs()->ISR & USART_ISR_RXNE_RXFNE)) {
            /* busy wait */
        }
        return static_cast<uint8_t>(regs()->RDR);
    }

    EMPP_STATIC_INLINE void enable_irq_tx() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }

    EMPP_STATIC_INLINE void disable_irq_tx() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
    }

    EMPP_STATIC_INLINE void enable_irq_rx() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
    }

    EMPP_STATIC_INLINE void disable_irq_rx() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
    }

    EMPP_STATIC_INLINE void enable_irq_idle() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_IDLEIE;
    }

    EMPP_STATIC_INLINE void disable_irq_idle() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_IDLEIE;
    }

    EMPP_STATIC_INLINE bool is_tc() EMPP_NOEXCEPT
    {
        // *** NOTE: 这里判断的是 TXE/TXFNF（发送寄存器空），
        // 不是 TC 所有位发送完成 ***
        return (regs()->ISR & USART_ISR_TXE_TXFNF) != 0U;
    }

    EMPP_STATIC_INLINE bool is_rc() EMPP_NOEXCEPT
    {
        // *** NOTE: 这里判断的是 RXNE/RXFNE（有数据可读） ***
        return (regs()->ISR & USART_ISR_RXNE_RXFNE) != 0U;
    }

    EMPP_STATIC_INLINE bool is_idle() EMPP_NOEXCEPT
    {
        return (regs()->ISR & USART_ISR_IDLE) != 0U;
    }

    EMPP_STATIC_INLINE void clear_tc() EMPP_NOEXCEPT
    {
        // 这里清的是 TXFE 标志（TXFIFO empty clear flag）
        regs()->ICR = USART_ICR_TXFECF;
    }

    EMPP_STATIC_INLINE void clear_idle() EMPP_NOEXCEPT
    {
        regs()->ICR = USART_ICR_IDLECF;
    }

    EMPP_STATIC_INLINE void enable_dma_tx() EMPP_NOEXCEPT
    {
        regs()->CR3 |= USART_CR3_DMAT;
    }

    EMPP_STATIC_INLINE void disable_dma_tx() EMPP_NOEXCEPT
    {
        regs()->CR3 &= ~USART_CR3_DMAT;
    }

    EMPP_STATIC_INLINE void enable_dma_rx() EMPP_NOEXCEPT
    {
        regs()->CR3 |= USART_CR3_DMAR;
    }

    EMPP_STATIC_INLINE void disable_dma_rx() EMPP_NOEXCEPT
    {
        regs()->CR3 &= ~USART_CR3_DMAR;
    }

    EMPP_STATIC_INLINE void enable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::enable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void disable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void enable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::enable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void disable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void enable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::enable_irq_ht();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void disable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable_irq_ht();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void config_dma_tx(const void    *buffer,
                                          const uint16_t length) EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
            DmaTx::clear_all_flags();
            DmaTx::configAddr(reinterpret_cast<uintptr_t>(&regs()->TDR),
                              reinterpret_cast<uintptr_t>(buffer), length);
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void config_dma_rx(const void    *buffer,
                                          const uint16_t length) EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable();
            DmaRx::clear_all_flags();

            DmaRx::configAddr(reinterpret_cast<uintptr_t>(&regs()->RDR),
                              reinterpret_cast<uintptr_t>(buffer), length);
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /*
     * 每次发生 TXE（使能 FIFO模式时为 TXFNF ）事件后，
     * 数据都从存储器移动到 USART_TDR。
     */
    EMPP_STATIC_INLINE void start_dma_tx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            regs()->ICR = USART_ICR_TCCF; // UART TC 标志清零
            DmaTx::enable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void stop_dma_tx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /*
     * 每次发生 RXNE（使能 FIFO 模式时为 RXFNE）事件后，
     * 数据都从 USART_RDR 加载到此存储区域
     */
    EMPP_STATIC_INLINE void start_dma_rx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {

            DmaRx::enable();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    EMPP_STATIC_INLINE void stop_dma_rx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }
};

} // namespace empp::stm32h7xx::uart

#endif
