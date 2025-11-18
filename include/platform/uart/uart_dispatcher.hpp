// uart_dispatcher.hpp
#pragma once

#include "empp/type.hpp"

namespace empp::platform::uart {

struct UartDispatcher
{

    static inline Callback tx_callbacks[8] = {nullptr};
    static inline Callback rx_callbacks[8] = {nullptr};

    EMPP_ALWAYS_INLINE static void
    register_callback_tx(const uint8_t id, const Callback cb) noexcept
    {
        tx_callbacks[id] = cb;
    }

    EMPP_ALWAYS_INLINE static void dispatch_tx(const uint8_t id) noexcept
    {
        if (tx_callbacks[id])
            tx_callbacks[id]();
    }

    EMPP_ALWAYS_INLINE static void
    register_callback_rx(const uint8_t id, const Callback cb) noexcept
    {
        rx_callbacks[id] = cb;
    }

    EMPP_ALWAYS_INLINE static void dispatch_rx(const uint8_t id) noexcept
    {
        if (rx_callbacks[id])
            rx_callbacks[id]();
    }
};

} // namespace empp::platform::uart