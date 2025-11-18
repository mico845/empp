#include "common_inc.h"

using Com1 = uart::U1;
using Led  = gpio::PC13;
static volatile uint8_t ch;

void callback_rx() { ch = Com1::read(); }

void Main()
{
    Com1::register_callback_rx(callback_rx);
    Com1::enable_rx_irq();

    while (true) {
        if (ch == 't') {
            Led::toggle();
            ch = 0;
        }
    }
}