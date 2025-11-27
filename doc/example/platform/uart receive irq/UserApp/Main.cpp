#include "common_inc.h"

using namespace board;

EMPP_RAM_ITCM void Main()
{
    Com1::enable_irq_rx();

    while (true) {
        if (ch == 't') {
            Led::toggle();
            ch = 0;
        }
    }
}