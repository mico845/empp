#pragma once

#include "empp_config.hpp"

#if defined(EMPP_CHIP_STM32H7)
    #include "stm32h7xx.h"
#elif defined(EMPP_CHIP_STM32G4)
    #include "stm32g4xx"
#endif
