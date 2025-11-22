// spi_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "spi.h"
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "stm32h7xx_ll_spi.h"

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

    template <typename T>
    EMPP_ALWAYS_INLINE static void write(const T       *buf,
                                         const uint16_t count) noexcept
    {
        MODIFY_REG(regs()->CR2, SPI_CR2_TSIZE, count);

        regs()->CR1 |= SPI_CR1_SPE;
        regs()->CR1 |= SPI_CR1_CSTART;

        const T *p      = buf;
        uint16_t remain = count;

        while (remain > 0U) {
            if (regs()->SR & SPI_SR_TXP) {
                *reinterpret_cast<volatile T *>(&regs()->TXDR) = *p;
                ++p;
                --remain;
            }
        }

        while (!(regs()->SR & SPI_SR_EOT)) {
        }

        regs()->IFCR |= (SPI_IFCR_EOTC | SPI_IFCR_TXTFC);

        regs()->CR1 &= ~SPI_CR1_SPE;
    }

    EMPP_ALWAYS_INLINE static void set_dataWidth(const uint8_t N) noexcept
    {
        regs()->CFG1 = ((regs()->CFG1 & (~(SPI_CFG1_DSIZE))) | (N - 1));
    }
};
} // namespace empp::stm32h7xx::spi

#endif
