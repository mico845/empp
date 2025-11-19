#include "common_inc.h"

using Com1 = uart::U1;

constexpr uint8_t str[]   = "hello\r\n";
constexpr uint8_t str_len = sizeof(str) - 1;

void callback_tx()
{
    static uint8_t tx_byte_nums = 0;
    if (tx_byte_nums < str_len) {
        Com1::write(str[tx_byte_nums++]);
    }
    else {
        Com1::disable_tx_irq();
    }
}

void Main()
{

    Com1::register_callback_tx(callback_tx);
    Com1::enable_tx_irq();

    while (true) {
    }
}