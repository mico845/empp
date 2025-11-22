// spi_impl.hpp
#pragma once
#include "platform/spi/spi_concept.hpp"

namespace empp::platform::spi {

template <SpiBackend Backend>
struct SpiImpl
{
    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        Backend::write(value);
    }
};
} // namespace empp::platform::spi