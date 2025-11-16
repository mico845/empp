// delay_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::delay {

template <typename T>
concept DelayBackend =
    requires(uint32_t us, uint16_t ms, uint16_t s, uint16_t sys_mhz) {
        {
            T::init()
        } noexcept;

        {
            T::init(sys_mhz)
        } noexcept;

        {
            T::us(us)
        } noexcept;
        {
            T::ms(ms)
        } noexcept;
        {
            T::s(s)
        } noexcept;
    };

} // namespace empp::platform::delay
