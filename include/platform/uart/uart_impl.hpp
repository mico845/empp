// uart_impl.hpp
#pragma once
#include "platform/uart/uart_concept.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
struct UartImpl
{
    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        Backend::write(value);
    }
};
} // namespace empp::platform::uart