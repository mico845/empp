// uart_impl.hpp
#pragma once
#include "empp_config.h"
#include "empp/assert.hpp"
#include "platform/uart/uart_concept.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
struct UartImpl
{
    /* OP */
    EMPP_STATIC_INLINE void write(uint8_t value) EMPP_NOEXCEPT
    {
        Backend::write(value);
    }

    [[nodiscard]] EMPP_STATIC_INLINE uint8_t read() EMPP_NOEXCEPT
    {
        return Backend::read();
    }

    /* IRQ */
    [[nodiscard]] EMPP_STATIC_INLINE bool is_tc() EMPP_NOEXCEPT
    {
        return Backend::is_tc();
    }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_rc() EMPP_NOEXCEPT
    {
        return Backend::is_rc();
    }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_idle() EMPP_NOEXCEPT
    {
        return Backend::is_idle();
    }

    EMPP_STATIC_INLINE void clear_tc() EMPP_NOEXCEPT { Backend::clear_tc(); }

    EMPP_STATIC_INLINE void clear_idle() EMPP_NOEXCEPT
    {
        Backend::clear_idle();
    }

    EMPP_STATIC_INLINE void enable_irq_tx() EMPP_NOEXCEPT
    {
        Backend::enable_irq_tx();
    }

    EMPP_STATIC_INLINE void disable_irq_tx() EMPP_NOEXCEPT
    {
        Backend::disable_irq_tx();
    }

    EMPP_STATIC_INLINE void enable_irq_rx() EMPP_NOEXCEPT
    {
        Backend::enable_irq_rx();
    }

    EMPP_STATIC_INLINE void disable_irq_rx() EMPP_NOEXCEPT
    {
        Backend::disable_irq_rx();
    }

    EMPP_STATIC_INLINE void enable_irq_idle() EMPP_NOEXCEPT
    {
        Backend::enable_irq_idle();
    }

    EMPP_STATIC_INLINE void disable_irq_idle() EMPP_NOEXCEPT
    {
        Backend::disable_irq_idle();
    }

    /* DMA TX */
    EMPP_STATIC_INLINE void enable_dma_tx() EMPP_NOEXCEPT
    {
        Backend::enable_dma_tx();
    }

    EMPP_STATIC_INLINE void disable_dma_tx() EMPP_NOEXCEPT
    {
        Backend::disable_dma_tx();
    }

    EMPP_STATIC_INLINE void enable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        Backend::enable_irq_dma_tx_tc();
    }

    EMPP_STATIC_INLINE void disable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        Backend::disable_irq_dma_tx_tc();
    }

    EMPP_STATIC_INLINE void config_dma_tx(const void *buffer,
                                          size_t      length) EMPP_NOEXCEPT
    {

#if defined(EMPP_DEBUG_TYPECHECK) && (EMPP_DEBUG_TYPECHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        /* STM32 H7 DMA len 仅 16 bit */
        EMPP_ASSERT(!(length > 0xFFFFu),
                    "STM32H7 DMA length exceeds 16-bit NDTR");

#endif

        Backend::config_dma_tx(buffer, length);
    }

    EMPP_STATIC_INLINE void start_dma_tx() EMPP_NOEXCEPT
    {
        Backend::start_dma_tx();
    }

    EMPP_STATIC_INLINE void stop_dma_tx() EMPP_NOEXCEPT
    {
        Backend::stop_dma_tx();
    }

    /* DMA RX */
    EMPP_STATIC_INLINE void enable_dma_rx() EMPP_NOEXCEPT
    {
        Backend::enable_dma_rx();
    }

    EMPP_STATIC_INLINE void disable_dma_rx() EMPP_NOEXCEPT
    {
        Backend::disable_dma_rx();
    }

    EMPP_STATIC_INLINE void config_dma_rx(const void *buffer,
                                          size_t      length) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_TYPECHECK) && (EMPP_DEBUG_TYPECHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        /* STM32 H7 DMA len 仅 16 bit */
        EMPP_ASSERT(!(length > 0xFFFFu),
                    "STM32H7 DMA length exceeds 16-bit NDTR");
#endif

        Backend::config_dma_rx(buffer, length);
    }

    EMPP_STATIC_INLINE void enable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        Backend::enable_irq_dma_rx_tc();
    }

    EMPP_STATIC_INLINE void disable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        Backend::disable_irq_dma_rx_tc();
    }

    EMPP_STATIC_INLINE void enable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        Backend::enable_irq_dma_rx_ht();
    }

    EMPP_STATIC_INLINE void disable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        Backend::disable_irq_dma_rx_ht();
    }

    EMPP_STATIC_INLINE void start_dma_rx() EMPP_NOEXCEPT
    {
        Backend::start_dma_rx();
    }

    EMPP_STATIC_INLINE void stop_dma_rx() EMPP_NOEXCEPT
    {
        Backend::stop_dma_rx();
    }
};

} // namespace empp::platform::uart
