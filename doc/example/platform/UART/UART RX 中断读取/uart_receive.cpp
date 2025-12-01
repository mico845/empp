#include "common_inc.h"

/// UART 中断 rx 读取

using namespace board;

void Main()
{
    Com1::enable_irq_rx(); // 👈 使能 UART rx 中断

    for (;;) {
        if (ch == 't') {
            Led::toggle();
            ch = 0;
        }
    }
}