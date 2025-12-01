#include "common_inc.h"

/// UART DMA TX 发送 + DMA 完成中断示例

using namespace board;

constexpr uint8_t      UART_TXFIFO_BYTES            = 24;
EMPP_RAM_SRAM1 uint8_t uart_data[UART_TXFIFO_BYTES] = {};  /* Write through, read allocate，no write allocate */

void Main()
{
    delay::init();
    constexpr char str[UART_TXFIFO_BYTES] = "[empp]:hello world!\r\n";

    for (size_t i = 0; i < UART_TXFIFO_BYTES; i++)
        uart_data[i] = str[i];

    Com1::enable_dma_tx();                                    // 👈 允许 UART 通过 DMA 发送数据
    Com1::config_dma_tx(uart_data, UART_TXFIFO_BYTES);      // 👈 配置 DMA 传输地址与长度
    Com1::enable_irq_dma_tx_tc();                             // 👈 使能 DMA TX 传输完成中断
    Com1::start_dma_tx();                                     // 👈 启动 TX 方向 DMA 传输

    for (;;) {
        if (uart_flag) {
            Com1::stop_dma_tx();                              // 👈 停止 TX 方向 DMA 传输

            uart_flag = false;

            Led::toggle();
            delay::s(1);
            Com1::start_dma_tx();
        }
    }
}