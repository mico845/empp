#include "common_inc.h"

/// UART DMA TX 发送 + Ring Fifo 示例

using namespace board;

constexpr uint8_t UART_TXFIFO_BYTES = 128;
EMPP_RAM_AXI_SRAM empp::fifo<uint8_t, UART_TXFIFO_BYTES>
                  uart_fifo; /* Write back, Read allocate，Write allocate */

static void send_data()
{
    size_t len = 0;
    while ((len = uart_fifo.linear_read_length())
           > 0) /* 👈 FIFO 获取线性可读数据长度 */
    {
        const auto data =
            uart_fifo.linear_read_ptr(); // 👈 FIFO 获取线性可读数据指针

        Com1::config_dma_tx(data, len); // 👈 配置 DMA 传输地址与长度
        Com1::start_dma_tx();           // 👈 启动 TX 方向 DMA 传输

        uart_fifo.skip(len); // 👈 FIFO 跳过已读数据
    }
}

void Main()
{
    constexpr uint8_t str[] = "[empp]:hello world!\r\n";

    uart_fifo.write(str, sizeof(str));
    cache::clean_obj<empp::fifo<uint8_t, UART_TXFIFO_BYTES>>(
        uart_fifo); // 👈 clean DCache

    Com1::enable_dma_tx(); // 👈 允许 UART 通过 DMA 发送数据
    send_data();

    for (;;) {
        __NOP();
    }
}