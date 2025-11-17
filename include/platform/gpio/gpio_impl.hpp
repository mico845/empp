// gpio_impl.hpp
#pragma once
#include "gpio_concept.hpp"

namespace empp::platform::gpio {

template <GpioBackend Backend>
struct GpioImpl
{
    EMPP_ALWAYS_INLINE static void set() noexcept { Backend::set(); }

    EMPP_ALWAYS_INLINE static void reset() noexcept { Backend::reset(); }

    EMPP_ALWAYS_INLINE static void toggle() noexcept { Backend::toggle(); }

    EMPP_ALWAYS_INLINE static bool read() noexcept { return Backend::read(); }
};

} // namespace empp::platform::gpio
