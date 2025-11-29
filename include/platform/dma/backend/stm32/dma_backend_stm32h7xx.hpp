// dma_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "empp/type.hpp"
    #include "empp/driver.hpp"
    #include "empp/define.hpp"

namespace empp::stm32h7xx::dma {

enum class DMAId : uint8_t {
    Dma1 = 1,
    Dma2 = 2,
};

enum class StreamId : uint8_t {
    S0 = 0,
    S1 = 1,
    S2 = 2,
    S3 = 3,
    S4 = 4,
    S5 = 5,
    S6 = 6,
    S7 = 7,
};

template <DMAId Dma, StreamId Strm>
struct DmaBackend
{
    using length_type = uint16_t;

    static_assert(Dma == DMAId::Dma1 || Dma == DMAId::Dma2,
                  "Invalid DMAId for STM32H7 DmaBackend");
    static_assert(static_cast<uint8_t>(Strm) <= 7u,
                  "Invalid StreamId for STM32H7 DmaBackend");

    EMPP_STATIC_INLINE constexpr DMA_TypeDef *dma() EMPP_NOEXCEPT
    {
        if constexpr (Dma == DMAId::Dma1) {
            return DMA1;
        }
        else {
            return DMA2;
        }
    }

    EMPP_STATIC_INLINE constexpr DMA_Stream_TypeDef *stream() EMPP_NOEXCEPT
    {

        if constexpr (Dma == DMAId::Dma1) {
            if constexpr (Strm == StreamId::S0)
                return DMA1_Stream0;
            if constexpr (Strm == StreamId::S1)
                return DMA1_Stream1;
            if constexpr (Strm == StreamId::S2)
                return DMA1_Stream2;
            if constexpr (Strm == StreamId::S3)
                return DMA1_Stream3;
            if constexpr (Strm == StreamId::S4)
                return DMA1_Stream4;
            if constexpr (Strm == StreamId::S5)
                return DMA1_Stream5;
            if constexpr (Strm == StreamId::S6)
                return DMA1_Stream6;
            if constexpr (Strm == StreamId::S7)
                return DMA1_Stream7;
        }
        else {
            if constexpr (Strm == StreamId::S0)
                return DMA2_Stream0;
            if constexpr (Strm == StreamId::S1)
                return DMA2_Stream1;
            if constexpr (Strm == StreamId::S2)
                return DMA2_Stream2;
            if constexpr (Strm == StreamId::S3)
                return DMA2_Stream3;
            if constexpr (Strm == StreamId::S4)
                return DMA2_Stream4;
            if constexpr (Strm == StreamId::S5)
                return DMA2_Stream5;
            if constexpr (Strm == StreamId::S6)
                return DMA2_Stream6;
            if constexpr (Strm == StreamId::S7)
                return DMA2_Stream7;
        }

        __builtin_unreachable();
    }

    EMPP_STATIC_INLINE void configAddr(const uint32_t    peripheralAddr,
                                       const uint32_t    memoryAddr,
                                       const length_type length) EMPP_NOEXCEPT
    {
        auto * const s = stream();
        s->PAR         = peripheralAddr;
        s->M0AR        = memoryAddr;
        /*
         *  NDTR
         *  要传输的数据项数目（0 到65535）。

            只有在禁止数据流时，才能向此寄存器执行写操作。
            使能数据流后，此寄存器为只读，用于指示要传输的剩余数据项数。
            每次 DMA 传输后，此 寄存器将递减。

            传输完成后，此寄存器保持为零（数据流处于正常模式时），
            或者在以下情况下自动以先前编程的值重载：
            1. 以循环模式配置数据流时
            2. 通过将 EN 位置“1”来重新使能数据流时

            如果该寄存器的值为零，则即使使能数据流，也无法完成任何事务。
         */
        s->NDTR = length;
    }

    EMPP_STATIC_INLINE void enable() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_EN;
    }

    EMPP_STATIC_INLINE void disable() EMPP_NOEXCEPT
    {
        auto * const s = stream();
        s->CR &= ~DMA_SxCR_EN;
        while ((s->CR & DMA_SxCR_EN) != 0U) {
            // busy wait, usually only a few cycles
        }
    }

    EMPP_STATIC_INLINE void enable_irq_tc() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_TCIE;
    }

    EMPP_STATIC_INLINE void disable_irq_tc() EMPP_NOEXCEPT
    {
        stream()->CR &= ~DMA_SxCR_TCIE;
    }

    EMPP_STATIC_INLINE void enable_irq_ht() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_HTIE;
    }

    EMPP_STATIC_INLINE void disable_irq_ht() EMPP_NOEXCEPT
    {
        stream()->CR &= ~DMA_SxCR_HTIE;
    }

    EMPP_STATIC_INLINE void clear_all_flags() EMPP_NOEXCEPT
    {
        auto * const d = dma();

        if constexpr (Strm == StreamId::S0) {
            d->LIFCR = DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTEIF0
                       | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0;
        }
        else if constexpr (Strm == StreamId::S1) {
            d->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CHTIF1 | DMA_LIFCR_CTEIF1
                       | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1;
        }
        else if constexpr (Strm == StreamId::S2) {
            d->LIFCR = DMA_LIFCR_CTCIF2 | DMA_LIFCR_CHTIF2 | DMA_LIFCR_CTEIF2
                       | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2;
        }
        else if constexpr (Strm == StreamId::S3) {
            d->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3
                       | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;
        }
        else if constexpr (Strm == StreamId::S4) {
            d->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4
                       | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;
        }
        else if constexpr (Strm == StreamId::S5) {
            d->HIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5
                       | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;
        }
        else if constexpr (Strm == StreamId::S6) {
            d->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6
                       | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6;
        }
        else if constexpr (Strm == StreamId::S7) {
            d->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7
                       | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;
        }
    }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_tc() EMPP_NOEXCEPT
    {
        const auto * const d = dma();

        if constexpr (Strm == StreamId::S0) {
            return (d->LISR & DMA_LISR_TCIF0) != 0U;
        }
        else if constexpr (Strm == StreamId::S1) {
            return (d->LISR & DMA_LISR_TCIF1) != 0U;
        }
        else if constexpr (Strm == StreamId::S2) {
            return (d->LISR & DMA_LISR_TCIF2) != 0U;
        }
        else if constexpr (Strm == StreamId::S3) {
            return (d->LISR & DMA_LISR_TCIF3) != 0U;
        }
        else if constexpr (Strm == StreamId::S4) {
            return (d->HISR & DMA_HISR_TCIF4) != 0U;
        }
        else if constexpr (Strm == StreamId::S5) {
            return (d->HISR & DMA_HISR_TCIF5) != 0U;
        }
        else if constexpr (Strm == StreamId::S6) {
            return (d->HISR & DMA_HISR_TCIF6) != 0U;
        }
        else if constexpr (Strm == StreamId::S7) {
            return (d->HISR & DMA_HISR_TCIF7) != 0U;
        }
        return false;
    }

    EMPP_STATIC_INLINE void clear_tc() EMPP_NOEXCEPT
    {
        auto * const d = dma();

        if constexpr (Strm == StreamId::S0) {
            d->LIFCR = DMA_LIFCR_CTCIF0;
        }
        else if constexpr (Strm == StreamId::S1) {
            d->LIFCR = DMA_LIFCR_CTCIF1;
        }
        else if constexpr (Strm == StreamId::S2) {
            d->LIFCR = DMA_LIFCR_CTCIF2;
        }
        else if constexpr (Strm == StreamId::S3) {
            d->LIFCR = DMA_LIFCR_CTCIF3;
        }
        else if constexpr (Strm == StreamId::S4) {
            d->HIFCR = DMA_HIFCR_CTCIF4;
        }
        else if constexpr (Strm == StreamId::S5) {
            d->HIFCR = DMA_HIFCR_CTCIF5;
        }
        else if constexpr (Strm == StreamId::S6) {
            d->HIFCR = DMA_HIFCR_CTCIF6;
        }
        else if constexpr (Strm == StreamId::S7) {
            d->HIFCR = DMA_HIFCR_CTCIF7;
        }
    }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_ht() EMPP_NOEXCEPT
    {
        const auto * const d = dma();

        if constexpr (Strm == StreamId::S0) {
            return (d->LISR & DMA_LISR_HTIF0) != 0U;
        }
        else if constexpr (Strm == StreamId::S1) {
            return (d->LISR & DMA_LISR_HTIF1) != 0U;
        }
        else if constexpr (Strm == StreamId::S2) {
            return (d->LISR & DMA_LISR_HTIF2) != 0U;
        }
        else if constexpr (Strm == StreamId::S3) {
            return (d->LISR & DMA_LISR_HTIF3) != 0U;
        }
        else if constexpr (Strm == StreamId::S4) {
            return (d->HISR & DMA_HISR_HTIF4) != 0U;
        }
        else if constexpr (Strm == StreamId::S5) {
            return (d->HISR & DMA_HISR_HTIF5) != 0U;
        }
        else if constexpr (Strm == StreamId::S6) {
            return (d->HISR & DMA_HISR_HTIF6) != 0U;
        }
        else if constexpr (Strm == StreamId::S7) {
            return (d->HISR & DMA_HISR_HTIF7) != 0U;
        }
        return false;
    }

    EMPP_STATIC_INLINE void clear_ht() EMPP_NOEXCEPT
    {
        auto * const d = dma();

        if constexpr (Strm == StreamId::S0) {
            d->LIFCR = DMA_LIFCR_CHTIF0;
        }
        else if constexpr (Strm == StreamId::S1) {
            d->LIFCR = DMA_LIFCR_CHTIF1;
        }
        else if constexpr (Strm == StreamId::S2) {
            d->LIFCR = DMA_LIFCR_CHTIF2;
        }
        else if constexpr (Strm == StreamId::S3) {
            d->LIFCR = DMA_LIFCR_CHTIF3;
        }
        else if constexpr (Strm == StreamId::S4) {
            d->HIFCR = DMA_HIFCR_CHTIF4;
        }
        else if constexpr (Strm == StreamId::S5) {
            d->HIFCR = DMA_HIFCR_CHTIF5;
        }
        else if constexpr (Strm == StreamId::S6) {
            d->HIFCR = DMA_HIFCR_CHTIF6;
        }
        else if constexpr (Strm == StreamId::S7) {
            d->HIFCR = DMA_HIFCR_CHTIF7;
        }
    }

    [[nodiscard]] EMPP_STATIC_INLINE length_type get_length() EMPP_NOEXCEPT
    {
        return stream()->NDTR & DMA_SxNDT;
    }
};

} // namespace empp::stm32h7xx::dma

#endif // EMPP_CHIP_STM32H7
