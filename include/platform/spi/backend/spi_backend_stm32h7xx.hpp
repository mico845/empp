// spi_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "empp/define.hpp"

namespace empp::stm32h7xx::spi {

template <uint8_t SpiId>
class SPIBackend
{
public:
    static_assert(SpiId >= 1 && SpiId <= 6, "Invalid SpiId for STM32H7");

    EMPP_STATIC_INLINE constexpr SPI_TypeDef *regs() EMPP_NOEXCEPT
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

        __builtin_unreachable();
    }

    template <typename T>
    EMPP_STATIC_INLINE void write(const T val) EMPP_NOEXCEPT
    {
        if constexpr (sizeof(T) == 1) {
            write<uint8_t>(&val, 1);
        }
        else if constexpr (sizeof(T) == 2) {
            write<uint16_t>(&val, 1);
        }
        else {
            static_assert(sizeof(T) == 1 || sizeof(T) == 2,
                          "SPIBackend::write(): Unsupported width");
        }
    }

    EMPP_STATIC_INLINE void write(const uint8_t *buf, const uint16_t count) EMPP_NOEXCEPT
    {
        write<uint8_t>(buf, count);
    }

    EMPP_STATIC_INLINE void write(const uint16_t *buf, const uint16_t count) EMPP_NOEXCEPT
    {
        write<uint16_t>(buf, count);
    }

    EMPP_STATIC_INLINE void set_dataWidth(const uint8_t N) EMPP_NOEXCEPT
    {
        auto * const   s          = regs();
        const uint32_t dsize_bits = static_cast<uint32_t>(N - 1U) << SPI_CFG1_DSIZE_Pos;
        s->CFG1                   = (s->CFG1 & ~SPI_CFG1_DSIZE) | dsize_bits;
    }

private:
    template <typename T>
    EMPP_STATIC_INLINE void write(const T *buf, const uint16_t count) EMPP_NOEXCEPT
    {
        static_assert(std::is_trivially_copyable_v<T>,
                      "SPIBackend::write<T>: T must be trivially copyable");

        auto * const s = regs();

        const uint16_t tsize_bit = static_cast<uint16_t>(count) << SPI_CR2_TSIZE_Pos;
        s->CR2                   = (s->CR2 & ~SPI_CR2_TSIZE) | tsize_bit;

        // 使能 SPI 和主机通信
        s->CR1 |= SPI_CR1_SPE;
        s->CR1 |= SPI_CR1_CSTART;

        const T *p      = buf;
        uint16_t remain = count;

        while (remain-- != 0U) {
            while ((s->SR & SPI_SR_TXP) == 0U) {
                __NOP();
            }
            *reinterpret_cast<volatile T *>(&s->TXDR) = *p++;
        }

        while ((s->SR & SPI_SR_EOT) == 0U) { // EOT: End of transfer
            // busy wait
        }

        // 清除 EOT/TXTF 标志，为下次传输准备
        s->IFCR = SPI_IFCR_EOTC | SPI_IFCR_TXTFC;

        // 关闭 SPI（按 RM，配置改变需在 SPE=0 时进行）
        s->CR1 &= ~SPI_CR1_SPE;
    }
};
} // namespace empp::stm32h7xx::spi

#endif
