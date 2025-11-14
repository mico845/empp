#pragma once
#include "gpio_ports.hpp"

namespace empp::platform::gpio {

template <typename Port, int Pin>
struct Gpio
{
    static_assert(Pin >= 0 && Pin < 16, "Pin must be 0~15");

    static constexpr uint32_t mask = (1u << Pin);

    /// Set pin high
    static void set() { Port::regs()->BSRR = mask; }

    /// Set pin low
    static void reset() { Port::regs()->BSRR = (mask << 16); }

    /// Toggle pin
    static void toggle()
    {
        if (auto regs = Port::regs(); regs->ODR & mask)
            regs->BSRR = (mask << 16);
        else
            regs->BSRR = mask;
    }

    /// Read input level
    static bool read() { return (Port::regs()->IDR & mask) != 0; }
};

} // namespace empp::platform::gpio
