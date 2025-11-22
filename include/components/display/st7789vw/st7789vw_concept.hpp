// st7789vw_concept.hpp
#pragma once

#include "empp/type.hpp"

namespace empp::componentBase::lcd::st7789vw {

template <typename T>
concept Pinx = requires {
    {
        T::set()
    } noexcept;
    {
        T::reset()
    } noexcept;
};

template <typename T>
concept Spix = requires(uint8_t v) {
    {
        T::write(v)
    } noexcept;
};

template <typename T>
concept Delayx = requires(uint16_t ms) {
    {
        T::ms(ms)
    } noexcept;
};

} // namespace empp::componentBase::lcd::st7789vw