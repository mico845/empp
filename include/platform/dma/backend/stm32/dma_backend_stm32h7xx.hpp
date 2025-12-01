// dma_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "empp/type.hpp"
    #include "empp/driver.hpp"
    #include "empp/define.hpp"

namespace empp::stm32h7xx::dma {

/**
 * @brief STM32H7 DMA 控制器 ID。
 *
 * @details
 * H7 系列通常包含 DMA1 和 DMA2 两个控制器。
 */
enum class DMAId : uint8_t {
    Dma1 = 1,
    Dma2 = 2,
};

/**
 * @brief STM32H7 DMA Stream ID。
 *
 * @details
 * 每个 DMA 控制器包含 8 个 Stream，编号为 0..7。
 */
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

/**
 * @brief STM32H7 通用 DMA 数据流 Backend(仅DMA1、DMA2)
 *
 * @tparam Dma  DMA 控制器 ID（DMA1 / DMA2）。
 * @tparam Strm DMA Stream ID（S0..S7）。
 *
 * @warning
 * STM32H7 DMA1 和 DMA2 无法访问 TCM 内存域
 */
template <DMAId Dma, StreamId Strm>
struct DmaBackend
{
    /// @brief 传输长度类型，STM32H7 NDTR 为 16 bit。
    using length_type = uint16_t;

    static_assert(Dma == DMAId::Dma1 || Dma == DMAId::Dma2,
                  "Invalid DMAId for STM32H7 DmaBackend");
    static_assert(static_cast<uint8_t>(Strm) <= 7u,
                  "Invalid StreamId for STM32H7 DmaBackend");

    /**
     * @brief 获取 DMA 控制器寄存器基址。
     *
     * @return 对应 DMA_TypeDef*（DMA1 或 DMA2）。
     */
    EMPP_STATIC_INLINE constexpr DMA_TypeDef *dma() EMPP_NOEXCEPT
    {
        if constexpr (Dma == DMAId::Dma1) {
            return DMA1;
        }
        else {
            return DMA2;
        }
    }

    /**
     * @brief 获取 DMA Stream 寄存器基址。
     *
     * @return 对应 DMA_Stream_TypeDef*（DMAx_Streamy）。
     */
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
    /**
     * @brief 配置外设地址、存储器地址和传输长度。
     *
     * @param peripheralAddr 外设寄存器地址（PAR）。
     * @param memoryAddr     存储器地址（M0AR）。
     * @param length         要传输的元素个数（NDTR，0-65535）。
     *
     * @details
     * - 仅在 EN=0（流禁用）时调用，符合参考手册要求：
     *   - NDTR / PAR / M0AR 在流禁用时可写；
     *   - 使能后只能通过 NDTR 读取剩余传输数。
     * - 不改变 CR 中其它配置（数据宽度、递增模式、方向等）。
     *
     *
     * @warning
     * 调用前需要确保：
     * - length 仅有 16位大小；
     * - 已通过 @ref disable() 确保 EN=0。
     */
    // 注意：调用者应确保在 EN=0 时调用（参考 RM：NDTR/PAR/M0AR 在流禁用时可写）
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

    /**
     * @brief 使能 DMA 流（设置 EN=1）。
     *
     * @details
     * - 仅设置 DMA_SxCR.EN 位；
     * - 不检查当前标志位和错误状态。
     */
    EMPP_STATIC_INLINE void enable() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_EN;
    }

    /**
     * @brief 禁用 DMA 流，并等待 EN 清零。
     *
     * @details
     * - 清除 DMA_SxCR.EN 位；
     * - 轮询 EN 直到实际硬件清零后返回；
     * - 这样可以保证后续立即安全地写 NDTR / PAR / M0AR。
     */
    EMPP_STATIC_INLINE void disable() EMPP_NOEXCEPT
    {
        auto * const s = stream();
        s->CR &= ~DMA_SxCR_EN;

        // 在修改 NDTR/PAR/M0AR 等寄存器前轮询 EN 直到清零
        while ((s->CR & DMA_SxCR_EN) != 0U) {
            // busy wait, usually only a few cycles
        }
    }

    /**
     * @brief 使能传输完成 (TC) 中断。
     */
    EMPP_STATIC_INLINE void enable_irq_tc() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_TCIE;
    }

    /**
     * @brief 失能传输完成 (TC) 中断。
     */
    EMPP_STATIC_INLINE void disable_irq_tc() EMPP_NOEXCEPT
    {
        stream()->CR &= ~DMA_SxCR_TCIE;
    }

    /**
     * @brief 使能半传输 (HT) 中断。
     */
    EMPP_STATIC_INLINE void enable_irq_ht() EMPP_NOEXCEPT
    {
        stream()->CR |= DMA_SxCR_HTIE;
    }

    /**
     * @brief 失能半传输 (HT) 中断。
     */
    EMPP_STATIC_INLINE void disable_irq_ht() EMPP_NOEXCEPT
    {
        stream()->CR &= ~DMA_SxCR_HTIE;
    }

    /**
     * @brief 清除当前 Stream 的所有相关标志位。
     *
     * @details
     * - 清除 LIFCR/HIFCR 中该 Stream 的：
     *   - 传输完成 (TCIFx)
     *   - 半传输 (HTIFx)
     *   - 传输错误 (TEIFx)
     *   - 直接模式错误 (DMEIFx)
     *   - FIFO 错误 (FEIFx)
     *
     * @note
     * 一般在配置和启动 DMA 之前调用一次，用于清理历史标志。
     */
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

    /**
     * @brief 查询传输完成 (TC) 标志。
     *
     * @retval true  当前 Stream 对应的 TCIFx 标志已置位。
     * @retval false TCIFx 未置位。
     *
     * @note
     * 标志位位于 LISR/HISR 中，不会自动清除，需要配合 @ref clear_tc() 使用。
     */
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

    /**
     * @brief 清除传输完成 (TC) 标志。
     *
     * @details
     * 向 LIFCR/HIFCR 中对应的 CTCIFx 写 1 清除 TCIFx。
     */
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

    /**
     * @brief 查询半传输 (HT) 标志。
     *
     * @retval true  当前 Stream 对应的 HTIFx 标志已置位。
     * @retval false HTIFx 未置位。
     */
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

    /**
     * @brief 清除半传输 (HT) 标志。
     *
     * @details
     * 向 LIFCR/HIFCR 中对应的 CHTIFx 写 1 清除 HTIFx。
     */
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

    /**
     * @brief 读取当前 NDTR（剩余传输项数）。
     *
     * @return NDTR 中的值，单位为“数据项数”（取决于配置的数据宽度）。
     *
     * @details
     * - 流使能期间 NDTR 为只读，表示剩余传输项；
     * - 流禁用时 NDTR 为重新装载的初始值。
     */
    [[nodiscard]] EMPP_STATIC_INLINE length_type get_length() EMPP_NOEXCEPT
    {
        return static_cast<length_type>(stream()->NDTR & DMA_SxNDT);
    }
};

} // namespace empp::stm32h7xx::dma

#endif // EMPP_CHIP_STM32H7
