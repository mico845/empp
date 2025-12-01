#include "common_inc.h"

/// UART 轮询接收示例

#include <empp.h>

using namespace empp::stm32h7xx;

using Led  = gpio::PC13;
using Com1 = uart::U1;

void Main()
{
    while (true) {
        if (const auto r = Com1::read(); r == 't') {    /* 👈 Com1::read() 读取一个 uint8_t */
            Led::toggle();
        }
    }
}