// empp.h
#pragma once

#include "empp_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/

    #if EMPP_CHIP_STM32H7
        #include "interface/platform/gpio/stm32/gpio_if_stm32h7xx.hpp"
        #include "interface/platform/delay/stm32/delay_if_stm32h7xx.hpp"
        #include "interface/platform/uart/stm32/uart_if_stm32h7xx.hpp"
        #include "interface/platform/spi/stm32/spi_if_stm32h7xx.hpp"
        #include "interface/platform/dma/stm32/dma_if_stm32h7xx.hpp"
    #else

    #endif

    #if EMPP_COMPONENTS_ON

        #include "interface/components/rgb_3gpio_if.hpp"

    #endif

#endif
