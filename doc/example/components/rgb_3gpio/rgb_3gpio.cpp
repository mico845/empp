#include "common_inc.h"

/// LED RGB 3GPIO 示例
using namespace empp::stm32h7xx;
using namespace empp::components;

using Led_R = gpio::PC0;
using Led_G = gpio::PC1;
using Led_B = gpio::PC2;

using Led_RGB = rgb_3gpio<Led_R, Led_G, Led_B>;

void Main()
{
    delay::init();
    Led_RGB::off();

    while (true) {
        Led_RGB::red();
        delay::s(1);
        Led_RGB::green();
        delay::s(1);
        Led_RGB::blue();
        delay::s(1);
        Led_RGB::yellow();
        delay::s(1);
        Led_RGB::cyan();
        delay::s(1);
        Led_RGB::pinkishRed();
        delay::s(1);
        Led_RGB::white();
        delay::s(1);
    }
}