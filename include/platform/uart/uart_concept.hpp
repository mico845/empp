// uart_concept.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::uart {

template <typename T>
concept UartBackend =
    std::is_empty_v<T> && requires(uint8_t value, const void *buf, size_t len) {
        // 基本收发
        {
            T::write(value)
        }
        EMPP_NOEXCEPT;
        {
            T::read()
        }
        EMPP_NOEXCEPT->std::same_as<uint8_t>;

        // 状态查询
        {
            T::is_tx()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;
        {
            T::is_tc()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;
        {
            T::is_rx()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;
        {
            T::is_idle()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;

        // 标志清除
        {
            T::clear_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::clear_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::clear_idle()
        }
        EMPP_NOEXCEPT;

        // IRQ
        {
            T::enable_irq_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_rx()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_rx()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_idle()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_idle()
        }
        EMPP_NOEXCEPT;

        // DMA TX
        {
            T::enable_dma_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_dma_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_dma_tx_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_dma_tx_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::config_dma_tx(buf, len)
        }
        EMPP_NOEXCEPT;
        {
            T::start_dma_tx()
        }
        EMPP_NOEXCEPT;
        {
            T::stop_dma_tx()
        }
        EMPP_NOEXCEPT;

        // DMA RX
        {
            T::enable_dma_rx()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_dma_rx()
        }
        EMPP_NOEXCEPT;
        {
            T::config_dma_rx(buf, len)
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_dma_rx_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_dma_rx_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_dma_rx_ht()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_dma_rx_ht()
        }
        EMPP_NOEXCEPT;
        {
            T::start_dma_rx()
        }
        EMPP_NOEXCEPT;
        {
            T::stop_dma_rx()
        }
        EMPP_NOEXCEPT;
    };

} // namespace empp::platform::uart
