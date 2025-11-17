// gpio_if_stm32h7xx.hpp
#pragma once
#include "empp_config.hpp"
#if defined(EMPP_CHIP_STM32H7)
    #include "platform/gpio/gpio_impl.hpp"
    #include "platform/gpio/backend/stm32/gpio_backend_stm32h7xx.hpp"
namespace empp::stm32h7xx::gpio {

constexpr uint8_t PORT_A = 0;
constexpr uint8_t PORT_B = 1;
constexpr uint8_t PORT_C = 2;
constexpr uint8_t PORT_D = 3;
constexpr uint8_t PORT_E = 4;

    #define GPIO_H7xx(Port, Pin)  \
        platform::gpio::GpioImpl< \
            Stm32H7Backend<platform::gpio::PinId(Port, Pin)>>;

// Port A Pins
using A0 = GPIO_H7xx(PORT_A, 0);

using C13 = GPIO_H7xx(PORT_C, 13);
} // namespace empp::stm32h7xx::gpio

#endif
