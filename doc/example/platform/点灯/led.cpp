#include "common_inc.h"

/// 点灯

#include <empp.h>

using namespace empp::stm32h7xx;

using Led  = gpio::PC13;
using Com1 = uart::U1;

void Main()
{
    delay::init(); // 👈 初始化延时（DWT / SysTick）
    Com1::println("LED blink start");

    while (true) {
        Led::toggle();  // 👈 翻转 LED 状态
        delay::ms(500); // 👈 延时 500ms
    }
}