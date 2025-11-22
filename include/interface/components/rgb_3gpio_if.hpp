// rgb_3gpio_if.hpp
#pragma once
#include "components/display/rgb_3gpio/rgb_3gpio.hpp"

namespace empp::components {

using RGBPolarity = componentBase::rgb_3gpio::RGBPolarity;

template <componentBase::rgb_3gpio::Pinx R, componentBase::rgb_3gpio::Pinx G,
          componentBase::rgb_3gpio::Pinx B,
          RGBPolarity                    Pol = RGBPolarity::CommonAnode>
using rgb_3gpio = componentBase::rgb_3gpio::RGB<R, G, B, Pol>;

} // namespace empp::components