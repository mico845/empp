// spi_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::spi {

template <typename T>
concept SpiBackend = requires(uint8_t *value_8, uint16_t count) {
    {
        T::template write<uint8_t>(value_8, count)
    } noexcept;
};

} // namespace empp::platform::spi
