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

static volatile uint16_t g_rx_write_pos = 0U;

EMPP_STATIC_INLINE void uart1_rx_update_from_dma() noexcept
{
    const uint16_t prev = g_rx_write_pos;

    const auto     dma_remaining = Uart1RxDma::get_length(); // 👈 获取 DMA 剩余传输长度
    const uint16_t curr          = UART_DMA_RX_BUFFER_SIZE - dma_remaining;

    if (curr == prev) {
        return; // 没有新数据
    }

    if (curr > prev) {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && (EMPP_USE_CACHE == 1U)
        cache::invalidate_ptr(&bufferRxDma[prev], curr - prev);
#endif
        fifoRxDma.write(&bufferRxDma[prev], curr - prev);
    }
    else {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && (EMPP_USE_CACHE == 1U)
        cache::invalidate_ptr(&bufferRxDma[prev], UART_DMA_RX_BUFFER_SIZE - prev);
        cache::invalidate_ptr(&bufferRxDma[0], curr);
#endif
        fifoRxDma.write(&bufferRxDma[prev], UART_DMA_RX_BUFFER_SIZE - prev);
        fifoRxDma.write(&bufferRxDma[0], curr);
    }

    g_rx_write_pos  = curr;
    g_rx_data_ready = true;
}

void USART1_IRQHandler()
{
    if (Com1::is_idle()) {
        uart1_rx_update_from_dma();
        g_rx_idle_event = true;
        Com1::clear_idle();
    }
}

void DMA2_Stream6_IRQHandler()
{
    if (Uart1RxDma::is_ht()) {
        uart1_rx_update_from_dma();
        Uart1RxDma::clear_ht();
    }
    if (Uart1RxDma::is_tc()) {
        uart1_rx_update_from_dma();
        Uart1RxDma::clear_tc();
    }
}