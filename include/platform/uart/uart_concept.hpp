#pragma once
#include "empp/type.hpp"

namespace empp::platform::uart {
template <typename T>
concept UartBackend = requires(uint8_t value, Callback cb) {
    {
        T::write(value)
    } noexcept;
    {
        T::read()
    } noexcept -> std::same_as<uint8_t>;
    {
        T::enable_tx_irq()
    } noexcept;
    {
        T::disable_tx_irq()
    } noexcept;
    {
        T::register_callback_tx(cb)
    } noexcept;
    {
        T::handle_irq()
    } noexcept;
};
} // namespace empp::platform::uart