// spi_impl.hpp
#pragma once
#include "platform/spi/spi_concept.hpp"
#include "empp/assert.hpp"
#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::spi {

template <SpiBackend Backend>
struct SpiImpl
{

    template <typename T>
    EMPP_STATIC_INLINE void write(const T val) EMPP_NOEXCEPT
    {
        static_assert(std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>,
                      "SpiImpl::write(T): T must be uint8_t or uint16_t");

        Backend::template write<T>(val);
    }

    EMPP_STATIC_INLINE void write(const uint8_t *buf, const size_t count) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(count > 0u, "spi write16() count is 0");
    #if defined(EMPP_CHIP_STM32H7)
        // STM32H7 SPI TSIZE 为 16bit
        EMPP_ASSERT(count <= 0xFFFFu, "STM32H7 SPI TSIZE (16-bit) overflow in write8()");
    #endif
#endif

        Backend::write(buf, count);
    }

    EMPP_STATIC_INLINE void write(const uint16_t *buf, const uint16_t count) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(count > 0u, "spi write16() count is 0");
    #if defined(EMPP_CHIP_STM32H7)
        // STM32H7 SPI TSIZE 为 16bit
        EMPP_ASSERT(count <= 0xFFFFu, "STM32H7 SPI TSIZE (16-bit) overflow in write16()");
    #endif

#endif
        Backend::write(buf, count);
    }

    EMPP_STATIC_INLINE void set_dataWidth(const uint8_t N) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U) && defined(EMPP_CHIP_STM32H7)
        // H7 DSIZE 有效范围：4 - 32
        EMPP_ASSERT(N >= 4U && N <= 32U, "SPI data width N out of range (4-32bit)");
#endif
        Backend::set_dataWidth(N);
    }
};
} // namespace empp::platform::spi