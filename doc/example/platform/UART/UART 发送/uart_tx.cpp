#include "common_inc.h"

/// UART 阻塞发送示例

#include <empp.h>
using namespace empp::stm32h7xx;

using Com1 = uart::U1;

EMPP_RAM_ITCM void Main()
{
    uint32_t num_32b = 0XF0F0F0F0U;
    int      num_neg = -1;
    char     str1[]  = "welcome to empp! ";
    char     str2[]  = "this is uart test. ";
    char     str3[]  = "let's go!";

    Com1::println("hello, empp");           // 👈 传输字符串
    Com1::println(str1, str2, str3);        // 👈 传输拼接字符串
    Com1::println("num(32b) = ", num_32b);  // 👈 传输无符号类型
    Com1::println("num(int) = ", num_neg);  // 👈 传输有符号类型

    for (uint8_t buf[3] = {0xAB, 0xCD, 0xEF}; auto c : buf)
        Com1::write(c);                     // 👈 逐 Byte 发送

    for (;;) {
        __NOP();
    }
}