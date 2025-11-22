// spi_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"

namespace empp::stm32h7xx::spi {

template <uint8_t SpiId>
struct SPIBackend
{
    EMPP_ALWAYS_INLINE static SPI_TypeDef *regs() noexcept
    {
        if constexpr (SpiId == 1)
            return SPI1;
        if constexpr (SpiId == 2)
            return SPI2;
        if constexpr (SpiId == 3)
            return SPI3;
        if constexpr (SpiId == 4)
            return SPI4;
        if constexpr (SpiId == 5)
            return SPI5;
        if constexpr (SpiId == 6)
            return SPI6;
        return SPI1;
    }

    EMPP_ALWAYS_INLINE static void write(const uint8_t *buf,
                                         uint16_t       count) noexcept
    {
        regs()->CR2 = (regs()->CR2 & ~SPI_CR2_TSIZE)
                      | (static_cast<uint32_t>(count) & SPI_CR2_TSIZE);

        regs()->CR1 |= SPI_CR1_SPE;
        regs()->CR1 |= SPI_CR1_CSTART;

        while (count > 0U) {
            if (regs()->SR & SPI_SR_TXP) {
                *reinterpret_cast<volatile uint8_t *>(&regs()->TXDR) = *buf;
                buf += sizeof(uint8_t);
                --count;
            }
        }

        while (!(regs()->SR & SPI_SR_EOT)) {
        }

        regs()->IFCR |= (SPI_IFCR_EOTC | SPI_IFCR_TXTFC);

        regs()->CR1 &= ~SPI_CR1_SPE;
    }

    EMPP_ALWAYS_INLINE static void write(const uint8_t value) noexcept
    {
        write(&value, 1);
    }
};
} // namespace empp::stm32h7xx::spi

#endif
