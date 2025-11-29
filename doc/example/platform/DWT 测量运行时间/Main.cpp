#include "common_inc.h"

using namespace empp::stm32h7xx;
using namespace empp::components;

using Com1 = uart::U1;

EMPP_RAM_ITCM void Main()
{
    delay::init();

    const uint32_t time_us = delay::measure_us([] {
        /* 测量运行开始时间 */
        gpio::PD12::toggle();
        delay::us(1);
        gpio::PD12::toggle();
        /* 测量运行结束时间 */
    });

    Com1::println("time:", time_us, "us");

    for (;;) {
    }
}