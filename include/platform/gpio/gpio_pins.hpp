// gpio_pins.hpp
#pragma once
#include "gpio_impl.hpp"
#include "gpio_ports.hpp"

namespace empp::platform::gpio {
// Port A Pins
using A0  = GpioImpl<PortA, 0>;
using A1  = GpioImpl<PortA, 1>;
using A2  = GpioImpl<PortA, 2>;
using A3  = GpioImpl<PortA, 3>;
using A4  = GpioImpl<PortA, 4>;
using A5  = GpioImpl<PortA, 5>;
using A6  = GpioImpl<PortA, 6>;
using A7  = GpioImpl<PortA, 7>;
using A8  = GpioImpl<PortA, 8>;
using A9  = GpioImpl<PortA, 9>;
using A10 = GpioImpl<PortA, 10>;
using A11 = GpioImpl<PortA, 11>;
using A12 = GpioImpl<PortA, 12>;
using A13 = GpioImpl<PortA, 13>;
using A14 = GpioImpl<PortA, 14>;
using A15 = GpioImpl<PortA, 15>;

// Port B Pins
using B0  = GpioImpl<PortB, 0>;
using B1  = GpioImpl<PortB, 1>;
using B2  = GpioImpl<PortB, 2>;
using B3  = GpioImpl<PortB, 3>;
using B4  = GpioImpl<PortB, 4>;
using B5  = GpioImpl<PortB, 5>;
using B6  = GpioImpl<PortB, 6>;
using B7  = GpioImpl<PortB, 7>;
using B8  = GpioImpl<PortB, 8>;
using B9  = GpioImpl<PortB, 9>;
using B10 = GpioImpl<PortB, 10>;
using B11 = GpioImpl<PortB, 11>;
using B12 = GpioImpl<PortB, 12>;
using B13 = GpioImpl<PortB, 13>;
using B14 = GpioImpl<PortB, 14>;
using B15 = GpioImpl<PortB, 15>;

// Port C Pins
using C0  = GpioImpl<PortC, 0>;
using C1  = GpioImpl<PortC, 1>;
using C2  = GpioImpl<PortC, 2>;
using C3  = GpioImpl<PortC, 3>;
using C4  = GpioImpl<PortC, 4>;
using C5  = GpioImpl<PortC, 5>;
using C6  = GpioImpl<PortC, 6>;
using C7  = GpioImpl<PortC, 7>;
using C8  = GpioImpl<PortC, 8>;
using C9  = GpioImpl<PortC, 9>;
using C10 = GpioImpl<PortC, 10>;
using C11 = GpioImpl<PortC, 11>;
using C12 = GpioImpl<PortC, 12>;
using C13 = GpioImpl<PortC, 13>;
using C14 = GpioImpl<PortC, 14>;
using C15 = GpioImpl<PortC, 15>;

// Port D Pins
using D0  = GpioImpl<PortD, 0>;
using D1  = GpioImpl<PortD, 1>;
using D2  = GpioImpl<PortD, 2>;
using D3  = GpioImpl<PortD, 3>;
using D4  = GpioImpl<PortD, 4>;
using D5  = GpioImpl<PortD, 5>;
using D6  = GpioImpl<PortD, 6>;
using D7  = GpioImpl<PortD, 7>;
using D8  = GpioImpl<PortD, 8>;
using D9  = GpioImpl<PortD, 9>;
using D10 = GpioImpl<PortD, 10>;
using D11 = GpioImpl<PortD, 11>;
using D12 = GpioImpl<PortD, 12>;
using D13 = GpioImpl<PortD, 13>;
using D14 = GpioImpl<PortD, 14>;
using D15 = GpioImpl<PortD, 15>;

// Port E Pins
using E0  = GpioImpl<PortE, 0>;
using E1  = GpioImpl<PortE, 1>;
using E2  = GpioImpl<PortE, 2>;
using E3  = GpioImpl<PortE, 3>;
using E4  = GpioImpl<PortE, 4>;
using E5  = GpioImpl<PortE, 5>;
using E6  = GpioImpl<PortE, 6>;
using E7  = GpioImpl<PortE, 7>;
using E8  = GpioImpl<PortE, 8>;
using E9  = GpioImpl<PortE, 9>;
using E10 = GpioImpl<PortE, 10>;
using E11 = GpioImpl<PortE, 11>;
using E12 = GpioImpl<PortE, 12>;
using E13 = GpioImpl<PortE, 13>;
using E14 = GpioImpl<PortE, 14>;
using E15 = GpioImpl<PortE, 15>;

} // namespace empp::platform::gpio
