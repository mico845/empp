#include "common_inc.h"

using Com1 = uart::U1;

void Main()
{
    delay::init();

    while (true) {
        Com1::println("hello world");
        delay::s(1);
    }
}