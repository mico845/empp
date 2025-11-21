// spi_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "platform/spi/spi_impl.hpp"

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

    EMPP_ALWAYS_INLINE static void enable() noexcept
    {
        regs()->CR1 |= SPI_CR1_SPE;
        regs()->CR1 |= SPI_CR1_CSTART;
    }

    EMPP_ALWAYS_INLINE static void disable() noexcept
    {
        regs()->CR1 &= ~SPI_CR1_SPE;
    }

    EMPP_ALWAYS_INLINE static void write(const uint32_t value) noexcept
    {
        while (!(regs()->SR & SPI_SR_TXP)) {
        }

        regs()->TXDR = value;

        while (!(regs()->SR & SPI_SR_TXC)) {
        }
    }
    EMPP_ALWAYS_INLINE static uint32_t read() noexcept
    {
        while (!(regs()->SR & SPI_SR_RXP)) {
        }

        return regs()->RXDR;
    }

    // FULL DUPLEX
    EMPP_ALWAYS_INLINE static uint32_t readWrite(const uint32_t value) noexcept
    {
        while (!(regs()->SR & SPI_SR_TXP)) {
        }
        regs()->TXDR = value;

        while (!(regs()->SR & SPI_SR_RXP)) {
        }
        return regs()->RXDR;
    }
};
} // namespace empp::stm32h7xx::spi

#endif
