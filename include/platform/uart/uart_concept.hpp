#pragma once

namespace empp::platform::uart {
template <typename T>
concept UartBackend = requires(uint8_t value) {
    {
        T::write(value)
    } noexcept;
};
} // namespace empp::platform::uart