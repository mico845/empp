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

template <uint8_t Port, uint8_t Pin>
using Gpio =
    platform::gpio::GpioImpl<GpioBackend<platform::gpio::PinId(Port, Pin)>>;

/* -----------------------------
 * GPIOA  (A0 - A15)
 * ----------------------------- */
using PA0  = Gpio<PORT_A, 0>;
using PA1  = Gpio<PORT_A, 1>;
using PA2  = Gpio<PORT_A, 2>;
using PA3  = Gpio<PORT_A, 3>;
using PA4  = Gpio<PORT_A, 4>;
using PA5  = Gpio<PORT_A, 5>;
using PA6  = Gpio<PORT_A, 6>;
using PA7  = Gpio<PORT_A, 7>;
using PA8  = Gpio<PORT_A, 8>;
using PA9  = Gpio<PORT_A, 9>;
using PA10 = Gpio<PORT_A, 10>;
using PA11 = Gpio<PORT_A, 11>;
using PA12 = Gpio<PORT_A, 12>;
using PA13 = Gpio<PORT_A, 13>;
using PA14 = Gpio<PORT_A, 14>;
using PA15 = Gpio<PORT_A, 15>;

/* -----------------------------
 * GPIOB  (B0 - B15)
 * ----------------------------- */
using PB0  = Gpio<PORT_B, 0>;
using PB1  = Gpio<PORT_B, 1>;
using PB2  = Gpio<PORT_B, 2>;
using PB3  = Gpio<PORT_B, 3>;
using PB4  = Gpio<PORT_B, 4>;
using PB5  = Gpio<PORT_B, 5>;
using PB6  = Gpio<PORT_B, 6>;
using PB7  = Gpio<PORT_B, 7>;
using PB8  = Gpio<PORT_B, 8>;
using PB9  = Gpio<PORT_B, 9>;
using PB10 = Gpio<PORT_B, 10>;
using PB11 = Gpio<PORT_B, 11>;
using PB12 = Gpio<PORT_B, 12>;
using PB13 = Gpio<PORT_B, 13>;
using PB14 = Gpio<PORT_B, 14>;
using PB15 = Gpio<PORT_B, 15>;

/* -----------------------------
 * GPIOC  (C0 - C15)
 * ----------------------------- */
using PC0  = Gpio<PORT_C, 0>;
using PC1  = Gpio<PORT_C, 1>;
using PC2  = Gpio<PORT_C, 2>;
using PC3  = Gpio<PORT_C, 3>;
using PC4  = Gpio<PORT_C, 4>;
using PC5  = Gpio<PORT_C, 5>;
using PC6  = Gpio<PORT_C, 6>;
using PC7  = Gpio<PORT_C, 7>;
using PC8  = Gpio<PORT_C, 8>;
using PC9  = Gpio<PORT_C, 9>;
using PC10 = Gpio<PORT_C, 10>;
using PC11 = Gpio<PORT_C, 11>;
using PC12 = Gpio<PORT_C, 12>;
using PC13 = Gpio<PORT_C, 13>;
using PC14 = Gpio<PORT_C, 14>;
using PC15 = Gpio<PORT_C, 15>;

/* -----------------------------
 * GPIOD  (D0 - D15)
 * ----------------------------- */
using PD0  = Gpio<PORT_D, 0>;
using PD1  = Gpio<PORT_D, 1>;
using PD2  = Gpio<PORT_D, 2>;
using PD3  = Gpio<PORT_D, 3>;
using PD4  = Gpio<PORT_D, 4>;
using PD5  = Gpio<PORT_D, 5>;
using PD6  = Gpio<PORT_D, 6>;
using PD7  = Gpio<PORT_D, 7>;
using PD8  = Gpio<PORT_D, 8>;
using PD9  = Gpio<PORT_D, 9>;
using PD10 = Gpio<PORT_D, 10>;
using PD11 = Gpio<PORT_D, 11>;
using PD12 = Gpio<PORT_D, 12>;
using PD13 = Gpio<PORT_D, 13>;
using PD14 = Gpio<PORT_D, 14>;
using PD15 = Gpio<PORT_D, 15>;

/* -----------------------------
 * GPIOE  (E0 - E15)
 * ----------------------------- */
using PE0  = Gpio<PORT_E, 0>;
using PE1  = Gpio<PORT_E, 1>;
using PE2  = Gpio<PORT_E, 2>;
using PE3  = Gpio<PORT_E, 3>;
using PE4  = Gpio<PORT_E, 4>;
using PE5  = Gpio<PORT_E, 5>;
using PE6  = Gpio<PORT_E, 6>;
using PE7  = Gpio<PORT_E, 7>;
using PE8  = Gpio<PORT_E, 8>;
using PE9  = Gpio<PORT_E, 9>;
using PE10 = Gpio<PORT_E, 10>;
using PE11 = Gpio<PORT_E, 11>;
using PE12 = Gpio<PORT_E, 12>;
using PE13 = Gpio<PORT_E, 13>;
using PE14 = Gpio<PORT_E, 14>;
using PE15 = Gpio<PORT_E, 15>;

} // namespace empp::stm32h7xx::gpio

#endif
