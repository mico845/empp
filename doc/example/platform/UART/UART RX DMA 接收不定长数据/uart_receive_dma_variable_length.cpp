#include "common_inc.h"

/// UART RX DMA 接收不定长数据示例

using namespace board;

uint8_t fifo_buf[UART_DMA_RX_FIFO_SIZE];

static void process_rx_data() noexcept
{

    const auto len = fifoRxDma.available_read();
    if (len == 0U) {
        return;
    }

    fifoRxDma.read(fifo_buf, len);
    Com1::print("\r\n$[empp Rx]:");
    Com1::print(fifo_buf, len);
    Com1::print("\r\n");
}

EMPP_RAM_ITCM void Main()
{
    delay::init();

    Com1::enable_dma_rx();
    Com1::enable_irq_dma_rx_ht(); // 👈 使能 DMA RX 传输半完成中断
    Com1::enable_irq_dma_rx_tc(); // 👈 使能 DMA RX 传输完成中断

    Com1::config_dma_rx(bufferRxDma, UART_DMA_RX_BUFFER_SIZE);
    Com1::start_dma_rx();

    delay::ms(20);
    Com1::clear_idle();      // 👈 UART初始化后会产生空闲帧，延时后再清除IDLE标志（可选）
    Com1::enable_irq_idle(); // 👈 开 RX IDLE 中断

    /*
     * - 数据量未达到半满，触发空闲中断
     * - 数据量达到半满，未达到满溢，先触发半满中断，后触发空闲中断
     * - 数据量刚好达到满溢，先触发半满中断，后触发满溢中断
     * - 数据量大于缓冲区长度，DMA循环覆盖溢出的字节
     */

    for (;;) {
        if (g_rx_idle_event) {
            g_rx_data_ready = false;
            g_rx_idle_event = false;

            process_rx_data(); // 处理这一次“帧结束”之前的全部数据
        }

        // 预留扩展点：如果未来希望“流式处理”，可以在此使用 g_rx_data_ready
        if (g_rx_data_ready) {
            /* something */
        }
    }
}