// spi_impl.hpp
#pragma once
#include "platform/spi/spi_concept.hpp"

namespace empp::platform::spi {

template <SpiBackend Backend>
struct SpiImpl
{
    EMPP_ALWAYS_INLINE static void write(const uint32_t value) noexcept
    {
        Backend::write(value);
    }
    EMPP_ALWAYS_INLINE static uint32_t read() noexcept
    {
        return Backend::read();
    }
    EMPP_ALWAYS_INLINE static uint32_t readWrite(const uint32_t value) noexcept
    {
        return Backend::readWrite(value);
    }

    EMPP_ALWAYS_INLINE static void enable() noexcept { Backend::enable(); }

    EMPP_ALWAYS_INLINE static void disable() noexcept { Backend::disable(); }
};
} // namespace empp::platform::spi