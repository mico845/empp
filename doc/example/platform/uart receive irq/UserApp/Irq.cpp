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

static void callback_rx() { ch = Com1::read(); }

void USART1_IRQHandler()
{
    if (Com1::is_rc()) {
        callback_rx();
    }
}
