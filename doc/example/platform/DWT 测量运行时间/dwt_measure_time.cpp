#include "common_inc.h"

/// DWT 测量运行时间示例

#include <empp.h>
using namespace empp::stm32h7xx;
using namespace empp::components;

using Com1 = uart::U1;

EMPP_RAM_ITCM void Main()
{
    delay::init();                          // 👈 初始化 (务必使用 DWT 即 DWT EMPP_DELAY_USE_DWT = 1)

    const uint32_t time_us = delay::measure_us([] {
        /* 测量运行开始时间 👈 */
        gpio::PD12::toggle();
        delay::us(1);
        gpio::PD12::toggle();
        /* 测量运行结束时间 👈 */
    });

    Com1::println("time:", time_us, "us");  // 👈 打印测量结果 time_us

    for (;;) {
    }
}