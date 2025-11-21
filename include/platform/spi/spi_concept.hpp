// spi_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::spi {

template <typename T>
concept SpiBackend = requires(uint32_t value) {
    {
        T::write(value)
    } noexcept;
    {
        T::read()
    } noexcept -> std::same_as<uint32_t>;
    {
        T::readWrite(value)
    } noexcept -> std::same_as<uint32_t>;
};

} // namespace empp::platform::spi
