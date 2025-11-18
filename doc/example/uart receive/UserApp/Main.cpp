#include "common_inc.h"

using Com1 = uart::U1;
using Led  = gpio::PC13;

void Main()
{
    while (true) {
        if (const auto r = Com1::read(); r == 't') {
            Led::toggle();
        }
    }
}