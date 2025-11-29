// delay_concept.hpp
#pragma once
#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::delay {

template <typename T>
concept DelayBackend = requires(size_t us, size_t ms, unsigned int sys_mhz) {
    {
        T::init(sys_mhz)
    }
    EMPP_NOEXCEPT;

    {
        T::us(us)
    }
    EMPP_NOEXCEPT;
    {
        T::ms(ms)
    }
    EMPP_NOEXCEPT;
};

} // namespace empp::platform::delay
