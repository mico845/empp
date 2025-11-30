#include "common_inc.h"

/// UART 中断 tx 发送

using namespace board;

void Main()
{
    Com1::enable_irq_tx(); // 👈 使能 UART tx 中断

    for (;;) {
        __NOP();
    }
}