// uart_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "empp/driver.hpp"
    #include "empp/type.hpp"
    #include "empp/define.hpp"
    #include "platform/dma/dma_concept.hpp"

namespace empp::stm32h7xx::uart {

/**
 * @brief STM32H7 UART 外设后端（Backend）。
 *
 * @tparam UartId UART 实例编号[1..6]，在编译期选择对应的 USARTx/UARTx
 * 寄存器组。
 * @tparam DmaTx  TX 方向使用的 DMA Backend 类型，必须满足
 * empp::platform::dma::DmaBackend， 或为 void 表示不使用 DMA TX。
 * @tparam DmaRx  RX 方向使用的 DMA Backend 类型，必须满足
 * empp::platform::dma::DmaBackend， 或为 void 表示不使用 DMA RX。
 */
template <uint8_t UartId, typename DmaTx = void, typename DmaRx = void>
struct UARTBackend
{
    static_assert(std::is_same_v<DmaTx, void>
                      || empp::platform::dma::DmaBackend<DmaTx>,
                  "DmaTx must satisfy empp::platform::dma::DmaBackend");
    static_assert(std::is_same_v<DmaRx, void>
                      || empp::platform::dma::DmaBackend<DmaRx>,
                  "DmaRx must satisfy empp::platform::dma::DmaBackend");

    /**
     * @brief 获取当前 UartId 对应的 USART 寄存器基址。
     *
     * @return 对应 USART_TypeDef* 寄存器指针。
     *
     * @warning
     * UartId 必须在 [1, 6] 范围内，否则会触发 static_assert。
     */
    EMPP_STATIC_INLINE constexpr USART_TypeDef *regs() EMPP_NOEXCEPT
    {
        static_assert(UartId >= 1 && UartId <= 6,
                      "Invalid UartId for STM32H7 UARTBackend");

        if constexpr (UartId == 1)
            return USART1;
        if constexpr (UartId == 2)
            return USART2;
        if constexpr (UartId == 3)
            return USART3;
        if constexpr (UartId == 4)
            return UART4;
        if constexpr (UartId == 5)
            return UART5;
        if constexpr (UartId == 6)
            return USART6;

        __builtin_unreachable();
    }

    /**
     * @brief 阻塞式发送 1 字节。
     *
     * @param value 要发送的字节。
     *
     * @details
     * 等待 TXE/TXFNF 置位（发送数据寄存器为空或 TX FIFO 未满），
     * 然后将 value 写入 TDR。
     * 本函数只保证“可以写下一个字节”，不保证所有比特已经发送完成（那是 TC
     * 的语义）。
     */
    EMPP_STATIC_INLINE void write(const uint8_t value) EMPP_NOEXCEPT
    {
        auto * const r = regs();
        while (!(r->ISR & USART_ISR_TXE_TXFNF)) {
            /* busy wait */
        }
        r->TDR = value;
    }
    /**
     * @brief 阻塞式接收 1 字节。
     *
     * @return 从 RDR 读取到的 8 位数据。
     *
     * @details
     * 等待 RXNE/RXFNE 置位（接收数据寄存器非空 / RX FIFO 非空），
     * 然后从 RDR 读取 1 字节。
     */
    EMPP_STATIC_INLINE uint8_t read() EMPP_NOEXCEPT
    {
        const auto * const r = regs();
        while (!(r->ISR & USART_ISR_RXNE_RXFNE)) {
            /* busy wait */
        }
        return static_cast<uint8_t>(r->RDR);
    }

    /**
     * @brief 使能 TXE/TXFNF 中断。
     *
     * @details
     * 设置 CR1 TXEIE/TXFNFIE 位，使能“发送数据寄存器为空 / TX FIFO 未满”中断，
     * 用于在中断服务函数中持续填充 TX FIFO。
     */
    EMPP_STATIC_INLINE void enable_irq_tx() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }

    /**
     * @brief 失能 TXE/TXFNF 中断。
     */
    EMPP_STATIC_INLINE void disable_irq_tx() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
    }

    /**
     * @brief 使能传输完成（TC）中断。
     *
     * @details
     * 设置 CR1 TCIE 位，传输完成时触发中断。
     * 将最后一个数据写入 USART_TDR 寄存器后，等待 TC =“1”：
     * – 禁止 FIFO 模式时，这表示最后一个帧的发送已完成。
     * – 使能 FIFO 模式时，这表示 TXFIFO 和移位寄存器均为空。
     */
    EMPP_STATIC_INLINE void enable_irq_tc() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_TCIE;
    }

    /**
     * @brief 失能传输完成（TC）中断。
     */
    EMPP_STATIC_INLINE void disable_irq_tc() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_TCIE;
    }

    /**
     * @brief 使能接收数据寄存器非空 / RX FIFO 非空中断。
     *
     * @details
     * 设置 CR1 RXNEIE/RXFNEIE 位，使能“接收数据寄存器非空/RXFIFO 非空”中断
     */
    EMPP_STATIC_INLINE void enable_irq_rx() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
    }

    /**
     * @brief 失能接收数据寄存器非空 / RX FIFO 非空中断。
     */
    EMPP_STATIC_INLINE void disable_irq_rx() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
    }

    /**
     * @brief 使能空闲（IDLE）中断。
     */
    EMPP_STATIC_INLINE void enable_irq_idle() EMPP_NOEXCEPT
    {
        regs()->CR1 |= USART_CR1_IDLEIE;
    }

    /**
     * @brief 失能空闲（IDLE）中断。
     */
    EMPP_STATIC_INLINE void disable_irq_idle() EMPP_NOEXCEPT
    {
        regs()->CR1 &= ~USART_CR1_IDLEIE;
    }

    /**
     * @brief 查询 TXE/TXFNF 状态。
     *
     * @retval true  TDR / TX FIFO 至少有一个空槽，可以写入下一个字节。
     * @retval false 发送数据寄存器已满，暂时不能写入新数据。
     *
     * @note
     * 该标志仅反映“是否可写下一个字节”，不代表最后一个 bit 已经发送完成。
     * 若要判断发送通道彻底空，应使用 is_tc()。
     */
    EMPP_STATIC_INLINE bool is_tx() EMPP_NOEXCEPT
    {
        return (regs()->ISR & USART_ISR_TXE_TXFNF) != 0U;
    }

    /**
     * @brief 查询传输完成（TC）状态。
     *
     * @retval true
     * 发送通道完全空（FIFO中无待发数据，移位寄存器也发送完最后一个bit）。
     * @retval false 仍有数据在发送或等待发送。
     */
    EMPP_STATIC_INLINE bool is_tc() EMPP_NOEXCEPT
    {
        return (regs()->ISR & USART_ISR_TC) != 0U;
    }

    /**
     * @brief 查询 RXNE/RXFNE 状态。
     *
     * @retval true  RX FIFO 中至少有一个字节可读。
     * @retval false 当前没有接收到新字节。
     */
    EMPP_STATIC_INLINE bool is_rx() EMPP_NOEXCEPT
    {
        return (regs()->ISR & USART_ISR_RXNE_RXFNE) != 0U;
    }

    /**
     * @brief 查询 IDLE（空闲）状态。
     *
     * @retval true  检测到空闲（超过一个数据帧无数据）。
     * @retval false 当前未检测到空闲条件。
     */
    EMPP_STATIC_INLINE bool is_idle() EMPP_NOEXCEPT
    {
        return (regs()->ISR & USART_ISR_IDLE) != 0U;
    }

    /**
     * @brief 清除传输完成（TC）标志。
     *
     * @details
     * 向 USART_ICR TCCF 写 1 清除 TC 标志。
     */
    EMPP_STATIC_INLINE void clear_tc() EMPP_NOEXCEPT
    {
        regs()->ICR = USART_ICR_TCCF;
    }

    /**
     * @brief 清除 TXFIFO 为空（TXFECF）标志。
     *
     * @details
     * 向 USART_ICR TXFECF 写 1 清除 TXFIFO 为空标志 (TXFIFO empty clear)。
     */
    EMPP_STATIC_INLINE void clear_tx() EMPP_NOEXCEPT
    {
        regs()->ICR = USART_ICR_TXFECF;
    }

    /**
     * @brief 清除空闲（IDLE）标志。
     *
     * @details
     * 向 USART_ICR IDLECF 写 1 清除 IDLE 标志。
     */
    EMPP_STATIC_INLINE void clear_idle() EMPP_NOEXCEPT
    {
        regs()->ICR = USART_ICR_IDLECF;
    }

    /**
     * @brief 使能 TX 方向 DMA 请求。
     *
     * @details
     * 设置 CR3 DMAT 位，允许 USART 通过 DMA 发送数据。
     */
    EMPP_STATIC_INLINE void enable_dma_tx() EMPP_NOEXCEPT
    {
        regs()->CR3 |= USART_CR3_DMAT;
    }

    /**
     * @brief 失能 TX 方向 DMA 请求。
     */
    EMPP_STATIC_INLINE void disable_dma_tx() EMPP_NOEXCEPT
    {
        regs()->CR3 &= ~USART_CR3_DMAT;
    }

    /**
     * @brief 使能 RX 方向 DMA 请求。
     *
     * @details
     * 设置 CR3.DMAR 位，允许 USART 通过 DMA 接收数据。
     */
    EMPP_STATIC_INLINE void enable_dma_rx() EMPP_NOEXCEPT
    {
        regs()->CR3 |= USART_CR3_DMAR;
    }

    /**
     * @brief 失能 RX 方向 DMA 请求。
     */
    EMPP_STATIC_INLINE void disable_dma_rx() EMPP_NOEXCEPT
    {
        regs()->CR3 &= ~USART_CR3_DMAR;
    }

    /**
     * @brief 使能 DMA TX 传输完成中断。
     *
     * @note 仅在 DmaTx 非 void 时可用，否则触发 static_assert。
     */
    EMPP_STATIC_INLINE void enable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::enable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /**
     * @brief 失能 DMA TX 传输完成中断。
     */
    EMPP_STATIC_INLINE void disable_irq_dma_tx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /**
     * @brief 使能 DMA RX 传输完成中断。
     */
    EMPP_STATIC_INLINE void enable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::enable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /**
     * @brief 失能 DMA RX 传输完成中断。
     */
    EMPP_STATIC_INLINE void disable_irq_dma_rx_tc() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable_irq_tc();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /**
     * @brief 使能 DMA RX 半传输（Half Transfer）中断。
     */
    EMPP_STATIC_INLINE void enable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::enable_irq_ht();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /**
     * @brief 失能 DMA RX 半传输（Half Transfer）中断。
     */
    EMPP_STATIC_INLINE void disable_irq_dma_rx_ht() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable_irq_ht();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /**
     * @brief 配置 TX 方向 DMA 传输地址与长度。
     *
     * @param buffer 指向发送缓冲区的内存地址。
     * @param length 要发送的元素个数。
     *
     * @details
     * - 外设地址固定为 USART_TDR；
     * - 调用前会先禁用 DMA 通道并清除所有 DMA 标志；
     * - 实际 NDTR 宽度为 16 bit，由上层在 DEBUG 下保证 length 不超过 0xFFFF。
     */
    EMPP_STATIC_INLINE void config_dma_tx(const void    *buffer,
                                          const uint16_t length) EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
            DmaTx::clear_all_flags();
            DmaTx::configAddr(reinterpret_cast<uintptr_t>(&regs()->TDR),
                              reinterpret_cast<uintptr_t>(buffer), length);
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /**
     * @brief 配置 RX 方向 DMA 传输地址与长度。
     *
     * @param buffer 指向接收缓冲区的内存地址。
     * @param length 要接收的元素个数。
     *
     * @details
     * - 外设地址固定为 USART_RDR；
     * - 调用前会先禁用 DMA 通道并清除所有 DMA 标志。
     * - 实际 NDTR 宽度为 16 bit，由上层在 DEBUG 下保证 length 不超过 0xFFFF。
     */
    EMPP_STATIC_INLINE void config_dma_rx(const void    *buffer,
                                          const uint16_t length) EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable();
            DmaRx::clear_all_flags();

            DmaRx::configAddr(reinterpret_cast<uintptr_t>(&regs()->RDR),
                              reinterpret_cast<uintptr_t>(buffer), length);
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }

    /**
     * @brief 启动 TX 方向 DMA 传输。
     *
     * @details
     * - 在启动 DMA 之前清除 UART TC 标志（ICR TCCF），
     *   以便在本次 DMA 发送完成后准确检测到 TC；
     * - 每次 TXE/TXFNF 事件发生时，DMA 会自动将数据从内存搬运到 USART_TDR。
     */
    EMPP_STATIC_INLINE void start_dma_tx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            regs()->ICR = USART_ICR_TCCF; // UART TC 标志清零
            DmaTx::enable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }
    /**
     * @brief 停止 TX 方向 DMA 传输。
     */
    EMPP_STATIC_INLINE void stop_dma_tx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaTx, void>) {
            DmaTx::disable();
        }
        else {
            static_assert(!std::is_same_v<DmaTx, void>,
                          "This UART backend has no Tx DMA configured");
        }
    }

    /**
     * @brief 启动 RX 方向 DMA 传输。
     *
     * @details
     * - 每次 RXNE/RXFNE 事件后，数据会自动从 USART_RDR 搬运到内存缓冲区；
     * - 常与 IDLE 中断或 DMA HT/TC 中断组合，实现不定长帧的接收。
     */
    EMPP_STATIC_INLINE void start_dma_rx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {

            DmaRx::enable();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }
    /**
     * @brief 停止 RX 方向 DMA 传输。
     */
    EMPP_STATIC_INLINE void stop_dma_rx() EMPP_NOEXCEPT
    {
        if constexpr (!std::is_same_v<DmaRx, void>) {
            DmaRx::disable();
        }
        else {
            static_assert(!std::is_same_v<DmaRx, void>,
                          "This UART backend has no Rx DMA configured");
        }
    }
};

} // namespace empp::stm32h7xx::uart

#endif
