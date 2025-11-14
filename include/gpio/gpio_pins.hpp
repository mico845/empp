#pragma once
#include "gpio.hpp"
#include "gpio_ports.hpp"

namespace empp::stm32h7::gpio {

// Port C Pins
using C0  = platform::gpio::Gpio<PortC, 0>;
using C1  = platform::gpio::Gpio<PortC, 1>;
using C2  = platform::gpio::Gpio<PortC, 2>;
using C3  = platform::gpio::Gpio<PortC, 3>;
using C4  = platform::gpio::Gpio<PortC, 4>;
using C5  = platform::gpio::Gpio<PortC, 5>;
using C6  = platform::gpio::Gpio<PortC, 6>;
using C7  = platform::gpio::Gpio<PortC, 7>;
using C8  = platform::gpio::Gpio<PortC, 8>;
using C9  = platform::gpio::Gpio<PortC, 9>;
using C10 = platform::gpio::Gpio<PortC, 10>;
using C11 = platform::gpio::Gpio<PortC, 11>;
using C12 = platform::gpio::Gpio<PortC, 12>;
using C13 = platform::gpio::Gpio<PortC, 13>;
using C14 = platform::gpio::Gpio<PortC, 14>;
using C15 = platform::gpio::Gpio<PortC, 15>;

} // namespace empp::stm32h7::gpio
