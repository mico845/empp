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

void DMA2_Stream6_IRQHandler()
{
    if (Uart1RxDma::is_tc()) {                                      // 👈 传输完成中断
        uart_flag = true;
        cache::invalidate_buf(uart_data, UART_RX_BYTES);      // 👈 invalidate Cache，保证读取到最新数据
        Uart1RxDma::clear_tc();                                     // 👈 手动清除传输完成标志位
    }
}