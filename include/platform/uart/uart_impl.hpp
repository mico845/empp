// uart_impl.hpp
#pragma once
#include "platform/uart/uart_concept.hpp"
#include "platform/uart/uart_dispatcher.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
struct UartImpl
{
    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        Backend::write(value);
    }
    EMPP_ALWAYS_INLINE static void enable_tx_irq() noexcept
    {
        Backend::enable_tx_irq();
    }

    EMPP_ALWAYS_INLINE static void disable_tx_irq() noexcept
    {
        Backend::disable_tx_irq();
    }

    EMPP_ALWAYS_INLINE static void
    register_callback_tx(const Callback cb) noexcept
    {
        Backend::register_callback_tx(cb);
    }

    EMPP_ALWAYS_INLINE static void handle_irq() noexcept
    {
        Backend::handle_irq();
    }
};
} // namespace empp::platform::uart