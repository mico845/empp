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

inline constexpr uint16_t UART_DMA_RX_BUFFER_SIZE = 8;
inline constexpr uint16_t UART_DMA_RX_FIFO_SIZE   = 128;

/* Write through, read allocate，no write allocate */
EMPP_RAM_SRAM1 inline uint8_t bufferRxDma[UART_DMA_RX_BUFFER_SIZE] = {};

inline empp::fifo<uint8_t, UART_DMA_RX_FIFO_SIZE> fifoRxDma; // // 👈 可选：环形缓冲区

// 有新数据到来（HT/TC/IDLE 任一触发）
inline volatile bool g_rx_data_ready = false;

// 一帧的结束（触发过 IDLE）
inline volatile bool g_rx_idle_event = false;
#endif
