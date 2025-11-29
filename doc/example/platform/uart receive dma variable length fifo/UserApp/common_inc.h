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

inline volatile bool            isRxFinished = false;
inline empp::fifo<uint8_t, 256> fifo_rx;

inline constexpr uint16_t     USART_DMA_RX_BUFFER_SIZE              = 128;
EMPP_RAM_SRAM1 inline uint8_t bufferRxDma[USART_DMA_RX_BUFFER_SIZE] = {};

#endif
