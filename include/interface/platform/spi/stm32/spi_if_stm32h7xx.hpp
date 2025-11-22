// spi_if_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/spi/backend/spi_backend_stm32h7xx.hpp"
    #include "platform/spi/spi_impl.hpp"

namespace empp::stm32h7xx::spi {

template <uint8_t SpiId>
using Spi = platform::spi::SpiImpl<SPIBackend<SpiId>>;

using S1 = Spi<1>;
using S2 = Spi<2>;
using S3 = Spi<3>;
using S4 = Spi<4>;
using S5 = Spi<5>;
using S6 = Spi<6>;

} // namespace empp::stm32h7xx::spi

#endif
