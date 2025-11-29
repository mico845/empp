#include "common_inc.h"

using namespace board;

EMPP_RAM_ITCM void Main()
{
    delay::init();

    Com1::enable_dma_tx();
    Com1::enable_dma_rx();
    Com1::enable_irq_dma_tx_tc();
    Com1::enable_irq_dma_rx_ht();
    Com1::enable_irq_dma_rx_tc();

    Com1::config_dma_rx(bufferRxDma, USART_DMA_RX_BUFFER_SIZE);
    Com1::start_dma_rx();

    Com1::enable_irq_idle();

    while (true) {
        if (isRxFinished) {
            isRxFinished = false;

            Led::toggle();
            while (!fifo_rx.empty()) {
                if (uint8_t num; fifo_rx.pop(num))
                    Com1::write(num);
            }
        }
    }
}