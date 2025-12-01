#include "common_inc.h"

/// UART DMA RX 接收定长数据示例

using namespace board;

void Main()
{
    Com1::enable_dma_rx();                                  // 👈 允许 UART 通过 DMA 接收数据
    Com1::config_dma_rx(uart_data, UART_RX_BYTES);    // 👈 配置 DMA 传输地址与长度
    Com1::enable_irq_dma_rx_tc();                           // 👈 使能 DMA RX 传输完成中断
    Com1::start_dma_rx();                                   // 👈 启动 RX 方向 DMA 传输

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_rx();                            // 👈 停止 RX 方向 DMA 传输
            uart_flag = false;

            Led::toggle();
            Com1::print(uart_data, UART_RX_BYTES);
            delay::s(1);

            Com1::start_dma_rx();
        }
    }
}