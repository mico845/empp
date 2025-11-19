// rgb_3gpio_concept.hpp
#pragma once

namespace empp::componentBase::rgb_3gpio {

template <typename T>
concept Pinx = requires {
    {
        T::set()
    } noexcept;
    {
        T::reset()
    } noexcept;
};
} // namespace empp::componentBase::rgb_3gpio