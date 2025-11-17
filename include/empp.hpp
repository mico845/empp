#pragma once

#include "empp_config.hpp"

#if EMPP_CHIP_STM32H7
    #include "interface/gpio/gpio_if_stm32h7xx.hpp"
    #include "interface/delay/delay_if_stm32h7xx.hpp"
    #include "interface/uart/uart_if_stm32h7xx.hpp"
#else

#endif
