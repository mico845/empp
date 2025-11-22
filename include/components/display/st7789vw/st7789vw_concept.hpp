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
concept Spix = requires(uint8_t v_8, uint16_t v_16) {
    {
        T::write8(v_8)
    } noexcept;
    {
        T::write16(v_16)
    } noexcept;
};

template <typename T>
concept Delayx = requires(uint16_t ms) {
    {
        T::ms(ms)
    } noexcept;
};

} // namespace empp::componentBase::lcd::st7789vw