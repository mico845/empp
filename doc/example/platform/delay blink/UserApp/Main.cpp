#include "common_inc.h"

using Led  = gpio::PC13;

void Main()
{
    delay::init();

    while (true) {
        Led::toggle();
        delay::ms(500);
    }
}