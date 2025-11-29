// uart_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::uart {

template <typename T>
concept UartBackend = requires(uint8_t value) {
    {
        T::write(value)
    }
    EMPP_NOEXCEPT;
    {
        T::read()
    }
    EMPP_NOEXCEPT->std::same_as<uint8_t>;
};

} // namespace empp::platform::uart
