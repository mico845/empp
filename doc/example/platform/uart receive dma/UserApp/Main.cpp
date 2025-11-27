#include "common_inc.h"

using namespace board;

constexpr uint8_t      uart_index = 6;
EMPP_RAM_SRAM1 uint8_t uart_data[uart_index];

EMPP_RAM_ITCM void Main()
{
    Com1::enable_dma_rx();
    Com1::config_dma_rx(uart_data, uart_index);
    Com1::enable_irq_dma_rx_tc();
    Com1::start_dma_rx();

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_rx();
            uart_flag = false;

            Led::toggle();
            for (const auto i : uart_data)
                Com1::write(i);
            delay::s(1);

            Com1::start_dma_rx();
        }
    }
}