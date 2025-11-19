#include "common_inc.h"

using Led_R = gpio::PC0;
using Led_G = gpio::PC1;
using Led_B = gpio::PC2;

using Led_RGB = rgb_3gpio<Led_R, Led_G, Led_B>;

void Main()
{
    delay::init();

    while (true) {
        Led_RGB::red();
        delay::ms(500);
        Led_RGB::green();
        delay::ms(500);
        Led_RGB::blue();
        delay::ms(500);
        Led_RGB::yellow();
        delay::ms(500);
        Led_RGB::cyan();
        delay::ms(500);
        Led_RGB::pinkishRed();
        delay::ms(500);
        Led_RGB::white();
        delay::ms(500);
    }
}