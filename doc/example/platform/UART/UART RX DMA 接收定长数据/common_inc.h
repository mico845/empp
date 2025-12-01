// common_inc.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

[[noreturn]] void Main();

#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/

    #include "board.hpp"
inline volatile bool uart_flag = false;

constexpr uint8_t UART_RX_BYTES = 6;

EMPP_RAM_SRAM1 inline uint8_t uart_data[UART_RX_BYTES] =
    {}; /* Write through, read allocate，no write allocate */
#endif
