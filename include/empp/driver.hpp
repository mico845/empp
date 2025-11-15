#pragma once

#include "empp_config.hpp"

#if defined(EMPP_CHIP_STM32H7)
    #include "stm32h7xx_ll_gpio.h"
    #include "stm32h7xx_ll_rcc.h"
    #include "stm32h7xx_ll_usart.h"
#elif defined(EMPP_CHIP_STM32F4)
    #include "stm32f4xx_ll_gpio.h"
#elif defined(EMPP_CHIP_STM32F7)
    #include "stm32f7xx_ll_gpio.h"
#endif
