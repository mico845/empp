#include "stm32h7xx_it.h"

#include "board.hpp"
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

constexpr uint8_t str[]   = "hello\r\n";
constexpr uint8_t str_len = sizeof(str) - 1;

static void callback_tx()
{
    static uint8_t tx_byte_nums = 0;
    if (tx_byte_nums < str_len) {
        Com1::write(str[tx_byte_nums++]);
    }
    else {
        Com1::disable_irq_tx();
    }
}

void USART1_IRQHandler()
{
    if (Com1::is_tc()) {
        callback_tx();
    }
}
