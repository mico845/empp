// gpio_concept.hpp
#pragma once
#include "empp/type.hpp"

namespace empp::platform::gpio {

template <typename T>
concept GpioBackend = requires {
    {
        T::set()
    } noexcept;
    {
        T::reset()
    } noexcept;
    {
        T::toggle()
    } noexcept;
    {
        T::read()
    } noexcept -> std::same_as<bool>;
};

} // namespace empp::platform::gpio
