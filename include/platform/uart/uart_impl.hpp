// uart_impl.hpp
#pragma once
#include "platform/uart/uart_concept.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
struct UartImpl
{

    /* OP */
    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        Backend::write(value);
    }

    EMPP_ALWAYS_INLINE static uint8_t read() noexcept
    {
        return Backend::read();
    }

    /* IRQ */
    EMPP_ALWAYS_INLINE static bool is_tc() noexcept { return Backend::is_tc(); }

    EMPP_ALWAYS_INLINE static bool is_rc() noexcept { return Backend::is_rc(); }

    EMPP_ALWAYS_INLINE static void enable_irq_tx() noexcept
    {
        Backend::enable_irq_tx();
    }

    EMPP_ALWAYS_INLINE static void disable_irq_tx() noexcept
    {
        Backend::disable_irq_tx();
    }

    EMPP_ALWAYS_INLINE static void enable_irq_rx() noexcept
    {
        Backend::enable_irq_rx();
    }

    EMPP_ALWAYS_INLINE static void disable_irq_rx() noexcept
    {
        Backend::disable_irq_rx();
    }

    /* DMA */
    EMPP_ALWAYS_INLINE static void enable_dma_tx() noexcept
    {
        Backend::enable_dma_tx();
    }

    EMPP_ALWAYS_INLINE static void disable_dma_tx() noexcept
    {
        Backend::disable_dma_tx();
    }

    EMPP_ALWAYS_INLINE static void enable_irq_dma_tx_tc() noexcept
    {
        Backend::enable_irq_dma_tx_tc();
    }

    EMPP_ALWAYS_INLINE static void disable_irq_dma_tx_tc() noexcept
    {
        Backend::disable_irq_dma_tx_tc();
    }

    EMPP_ALWAYS_INLINE static void config_dma_tx(const uint32_t buffer,
                                                 const uint32_t length) noexcept
    {
        Backend::config_dma_tx(buffer, length);
    }

    EMPP_ALWAYS_INLINE static void start_dma_tx() noexcept
    {
        Backend::start_dma_tx();
    }

    EMPP_ALWAYS_INLINE static void stop_dma_tx() noexcept
    {
        Backend::stop_dma_tx();
    }
};
} // namespace empp::platform::uart