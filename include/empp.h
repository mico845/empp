// empp.h
#pragma once

#include "empp_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/
#if EMPP_CHIP_STM32H7
    #include "interface/uart/stm32/uart_irq_dispatcher_stm32h7xx.h"
#else

#endif

#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/

    #if EMPP_CHIP_STM32H7
        #include "interface/gpio/stm32/gpio_if_stm32h7xx.hpp"
        #include "interface/delay/stm32/delay_if_stm32h7xx.hpp"
        #include "interface/uart/stm32/uart_if_stm32h7xx.hpp"
    #else

    #endif

#endif
