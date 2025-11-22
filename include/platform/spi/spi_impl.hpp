// spi_impl.hpp
#pragma once
#include "platform/spi/spi_concept.hpp"

namespace empp::platform::spi {

template <SpiBackend Backend>
struct SpiImpl
{
    EMPP_ALWAYS_INLINE static void write8(const uint8_t value) noexcept
    {
        Backend::template write<uint8_t>(&value, 1);
    }
    EMPP_ALWAYS_INLINE static void write8(const uint8_t *buf,
                                          const uint16_t count) noexcept
    {
        Backend::template write<uint8_t>(buf, count);
    }

    EMPP_ALWAYS_INLINE static void write16(const uint16_t value) noexcept
    {
        Backend::template write<uint16_t>(&value, 1);
    }
    EMPP_ALWAYS_INLINE static void write16(const uint16_t *buf,
                                           const uint16_t  count) noexcept
    {
        Backend::template write<uint16_t>(buf, count);
    }

    EMPP_ALWAYS_INLINE static void set_dataWidth(const uint8_t N) noexcept
    {
        Backend::set_dataWidth(N);
    }
};
} // namespace empp::platform::spi