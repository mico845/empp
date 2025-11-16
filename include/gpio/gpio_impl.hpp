// gpio_impl.hpp
#pragma once
#include "gpio_ports.hpp"

namespace empp::platform::gpio {

template <typename Port, uint8_t Pin>
struct GpioImpl
{
    static_assert(Pin < 16, "Pin must be 0~15");

    static constexpr uint32_t mask = (1u << Pin);

    EMPP_ALWAYS_INLINE static void set() noexcept { Port::regs()->BSRR = mask; }
    EMPP_ALWAYS_INLINE static void reset() noexcept
    {
        Port::regs()->BSRR = (mask << 16);
    }

    EMPP_ALWAYS_INLINE static void toggle() noexcept
    {
        if (auto regs = Port::regs(); regs->ODR & mask)
            regs->BSRR = (mask << 16);
        else
            regs->BSRR = mask;
    }

    EMPP_ALWAYS_INLINE static bool read() noexcept
    {
        return (Port::regs()->IDR & mask) != 0;
    }
};

} // namespace empp::platform::gpio
