#include "stm32h7xx_it.h"

#include "common_inc.h"

using namespace board;

void NMI_Handler()
{
    while (true) {
        /* something */
    }
}

void HardFault_Handler()
{
    while (true) {
        /* something */
    }
}

void MemManage_Handler()
{
    while (true) {
        /* something */
    }
}

void BusFault_Handler()
{
    while (true) {
        /* something */
    }
}

void UsageFault_Handler()
{
    while (true) {
        /* something */
    }
}

void SVC_Handler() { /* something */ }

void DebugMon_Handler() { /* something */ }

void PendSV_Handler() { /* something */ }

void SysTick_Handler() { /* something */ }

uint32_t last_dmarx_size = 0;

void USART1_IRQHandler()
{
    if (Com1::is_idle()) {
        // RX_DMA缓冲区长度 - dmarx_buf_remain_size
        const uint32_t recv_total_size =
            USART_DMA_RX_BUFFER_SIZE - Uart1RxDma::get_length();
        const uint32_t recv_size = recv_total_size - last_dmarx_size;

        if (recv_size > 0) {
            SCB_InvalidateDCache_by_Addr(&bufferRxDma[last_dmarx_size],
                                         static_cast<int32_t>(recv_size));

            fifo_rx.push(&bufferRxDma[last_dmarx_size], recv_size);
            last_dmarx_size = recv_total_size;
            isRxFinished    = true;
        }
        Com1::clear_idle();
    }
}

void DMA2_Stream6_IRQHandler()
{
    if (Uart1RxDma::is_ht()) {

        const uint32_t recv_total_size =
            USART_DMA_RX_BUFFER_SIZE - Uart1RxDma::get_length();
        const uint32_t recv_size = recv_total_size - last_dmarx_size;

        if (recv_size > 0) {
            SCB_InvalidateDCache_by_Addr(&bufferRxDma[last_dmarx_size],
                                         static_cast<int32_t>(recv_size));
            fifo_rx.push(&bufferRxDma[last_dmarx_size], recv_size);
            last_dmarx_size = recv_total_size;
        }

        Uart1RxDma::clear_ht();
    }
    if (Uart1RxDma::is_tc()) {

        const uint32_t recv_size = USART_DMA_RX_BUFFER_SIZE - last_dmarx_size;
        if (recv_size > 0) {
            SCB_InvalidateDCache_by_Addr(&bufferRxDma[last_dmarx_size],
                                         static_cast<int32_t>(recv_size));
            fifo_rx.push(&bufferRxDma[last_dmarx_size], recv_size);
        }
        last_dmarx_size = 0;
        Uart1RxDma::clear_tc();
    }
}
void DMA2_Stream7_IRQHandler()
{
    if (Uart1TxDma::is_tc()) {

        Uart1TxDma::clear_tc();
    }
}
