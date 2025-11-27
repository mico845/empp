#include "common_inc.h"
#include "board.hpp"

using namespace board;

constexpr uint8_t      uart_index = 20;
EMPP_RAM_SRAM1 uint8_t uart_data[uart_index];

void Main()
{
    delay::init();
    constexpr char str[uart_index] = "hello world!\r\n";
    for (size_t i = 0; i < uart_index; i++)
        uart_data[i] = str[i];

    Com1::enable_dma_tx();
    Com1::config_dma_tx(reinterpret_cast<uint32_t>(uart_data), uart_index);
    Com1::enable_irq_dma_tx_tc();
    Com1::start_dma_tx();

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_tx();

            uart_flag = false;

            Led::toggle();
            delay::s(1);
            Com1::start_dma_tx();
        }
    }
}