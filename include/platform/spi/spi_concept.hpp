// spi_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::spi {

template <typename T>
concept SpiBackend = requires(uint8_t value) {
    {
        T::write(value)
    } noexcept;
};

} // namespace empp::platform::spi
