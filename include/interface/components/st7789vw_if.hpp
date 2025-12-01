// st7789vw_if.hpp
#pragma once
#include "components/display/st7789vw/st7789vw.hpp"

namespace empp::components::lcd {

template <platform::spi::SpiBackend Spi, platform::gpio::GpioBackend PinDC,
          platform::gpio::GpioBackend PinBL, platform::delay::DelayBackend Delay>
using st7789vw = componentBase::lcd::st7789vw::ST7789VW<Spi, PinDC, PinBL, Delay>;

} // namespace empp::components::lcd