// gpio_impl.hpp
#pragma once

#include "empp/define.hpp"
#include "gpio_concept.hpp"

namespace empp::platform::gpio {

template <GpioBackend Backend>
struct GpioImpl
{
    EMPP_STATIC_INLINE void set() EMPP_NOEXCEPT { Backend::set(); }
    EMPP_STATIC_INLINE void reset() EMPP_NOEXCEPT { Backend::reset(); }
    EMPP_STATIC_INLINE void toggle() EMPP_NOEXCEPT { Backend::toggle(); }
    [[nodiscard]] EMPP_STATIC_INLINE bool read() EMPP_NOEXCEPT
    {
        return Backend::read();
    }
};

} // namespace empp::platform::gpio
