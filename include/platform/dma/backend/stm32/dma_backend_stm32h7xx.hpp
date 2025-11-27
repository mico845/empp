// dma_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "empp/type.hpp"
    #include "empp/driver.hpp"

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
    EMPP_ALWAYS_INLINE static DMA_TypeDef *dma() noexcept
    {
        if constexpr (Dma == DMAId::Dma1) {
            return DMA1;
        }
        else {
            return DMA2;
        }
    }

    EMPP_ALWAYS_INLINE static DMA_Stream_TypeDef *stream() noexcept
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
        return DMA1_Stream0;
    }

    EMPP_ALWAYS_INLINE static void configAddr(const uint32_t peripheralAddr,
                                              const uint32_t memoryAddr,
                                              const uint32_t length) noexcept
    {
        stream()->PAR  = peripheralAddr;
        stream()->M0AR = memoryAddr;
        stream()->NDTR = length;
    }

    EMPP_ALWAYS_INLINE static void enable() noexcept
    {
        stream()->CR |= DMA_SxCR_EN;
    }

    EMPP_ALWAYS_INLINE static void disable() noexcept
    {
        stream()->CR &= ~DMA_SxCR_EN;
    }
    EMPP_ALWAYS_INLINE static void enable_irq_tc() noexcept
    {
        stream()->CR |= DMA_SxCR_TCIE;
    }
    EMPP_ALWAYS_INLINE static void disable_irq_tc() noexcept
    {
        stream()->CR &= ~DMA_SxCR_TCIE;
    }

    EMPP_ALWAYS_INLINE static void clear_all_flags() noexcept
    {
        if constexpr (Strm == StreamId::S0) {
            dma()->LIFCR = DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0
                           | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0
                           | DMA_LIFCR_CFEIF0;
        }
        else if constexpr (Strm == StreamId::S1) {
            dma()->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CHTIF1
                           | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1
                           | DMA_LIFCR_CFEIF1;
        }
        else if constexpr (Strm == StreamId::S2) {
            dma()->LIFCR = DMA_LIFCR_CTCIF2 | DMA_LIFCR_CHTIF2
                           | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2
                           | DMA_LIFCR_CFEIF2;
        }
        else if constexpr (Strm == StreamId::S3) {
            dma()->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3
                           | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3
                           | DMA_LIFCR_CFEIF3;
        }
        else if constexpr (Strm == StreamId::S4) {
            dma()->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4
                           | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4
                           | DMA_HIFCR_CFEIF4;
        }
        else if constexpr (Strm == StreamId::S5) {
            dma()->HIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5
                           | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5
                           | DMA_HIFCR_CFEIF5;
        }
        else if constexpr (Strm == StreamId::S6) {
            dma()->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6
                           | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6
                           | DMA_HIFCR_CFEIF6;
        }
        else if constexpr (Strm == StreamId::S7) {
            dma()->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7
                           | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CDMEIF7
                           | DMA_HIFCR_CFEIF7;
        }
    }
    EMPP_ALWAYS_INLINE static bool is_tc() noexcept
    {
        if constexpr (Strm == StreamId::S0) {
            return (dma()->LISR & DMA_LISR_TCIF0) != 0U;
        }
        else if constexpr (Strm == StreamId::S1) {
            return (dma()->LISR & DMA_LISR_TCIF1) != 0U;
        }
        else if constexpr (Strm == StreamId::S2) {
            return (dma()->LISR & DMA_LISR_TCIF2) != 0U;
        }
        else if constexpr (Strm == StreamId::S3) {
            return (dma()->LISR & DMA_LISR_TCIF3) != 0U;
        }
        else if constexpr (Strm == StreamId::S4) {
            return (dma()->HISR & DMA_HISR_TCIF4) != 0U;
        }
        else if constexpr (Strm == StreamId::S5) {
            return (dma()->HISR & DMA_HISR_TCIF5) != 0U;
        }
        else if constexpr (Strm == StreamId::S6) {
            return (dma()->HISR & DMA_HISR_TCIF6) != 0U;
        }
        else if constexpr (Strm == StreamId::S7) {
            return (dma()->HISR & DMA_HISR_TCIF7) != 0U;
        }
        return false;
    }

    EMPP_ALWAYS_INLINE static void clear_tc() noexcept
    {
        if constexpr (Strm == StreamId::S0) {
            dma()->LIFCR = DMA_LIFCR_CTCIF0;
        }
        else if constexpr (Strm == StreamId::S1) {
            dma()->LIFCR = DMA_LIFCR_CTCIF1;
        }
        else if constexpr (Strm == StreamId::S2) {
            dma()->LIFCR = DMA_LIFCR_CTCIF2;
        }
        else if constexpr (Strm == StreamId::S3) {
            dma()->LIFCR = DMA_LIFCR_CTCIF3;
        }
        else if constexpr (Strm == StreamId::S4) {
            dma()->HIFCR = DMA_HIFCR_CTCIF4;
        }
        else if constexpr (Strm == StreamId::S5) {
            dma()->HIFCR = DMA_HIFCR_CTCIF5;
        }
        else if constexpr (Strm == StreamId::S6) {
            dma()->HIFCR = DMA_HIFCR_CTCIF6;
        }
        else if constexpr (Strm == StreamId::S7) {
            dma()->HIFCR = DMA_HIFCR_CTCIF7;
        }
    }
};

} // namespace empp::stm32h7xx::dma

#endif // EMPP_CHIP_STM32H7
