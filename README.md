# EMPP — Embedded Platform with C++

EMPP（Embedded Platform with C++） 是一款基于 STM32 的轻量级、追求高性能现代 C++ 嵌入式框架。

<p align="center">
  <img src="https://raw.githubusercontent.com/mico845/empp/main/doc/icon/empp.png" width="280">
</p>

核心理念：

- 🧵 低抽象开销：尽可能让功能均可在编译期内联展开
- ⚡ 接近裸寄存器的执行性能
- 🧑‍💻 C++20 风格的嵌入式编程

## 目前支持

### 外设驱动

| platform | H7xx (Test on H750VBT6) | G4xx |
|----------|-------------------------|------|
| `gpio`   | ✅                       | ?    |
| `delay`  | ✅                       | ?    |
| `uart`   | ✅                       | ?    |
| `dma`    | ✅                       | ?    |
| `spi`    | ING...                  | ?    |
| `iic`    | 计划中                     | ?    |

### 器件驱动

| components  | 说明               | 完成情况   |
|-------------|------------------|--------|
| `rgb_3gpio` | RGB灯（使用三个GPIO控制） | ✅      |
| `st7789vw`  | TFT-LCD屏幕驱动（SPI） | ING... |

---

## 📦 使用方法

1. 使用 STM32CubeMX 配置工程

   STM32CubeMX 作者使用版本：6.15.0

   完成配置后点击 `GENERATE CODE`

2. 选择编译器（GCC / Starm-Clang）

   作者使用版本：STM32CubeCLT_1.19.0

   Cmake Option 中通过 `CMAKE_TOOLCHAIN_FILE` 指定工具链（`cmake/starm-clang.cmake` / `cmake/gcc-arm-none-eabi.cmake`）
    ```shell
    -D CMAKE_TOOLCHAIN_FILE=cmake/starm-clang.cmake
    ```

3. 在主工程 CMakeLists.txt 中添加 EMPP

    ```cmake
    add_subdirectory(empp) # EMPP 文件夹路径
    
    target_link_libraries(${CMAKE_PROJECT_NAME}
    empp_platform
    )
    
    # Platform 依赖 STM32CubeMX 生成的 HAL/LL 头文件
   target_link_libraries(empp_platform INTERFACE stm32cubemx)
    ```

4. 引用 EMPP
    ```c++
    #include "empp.h"
    using namespace empp::stm32h7xx;    // 使用 EMPP 的 platform 型号
    using namespace empp::components;   // 使用 EMPP 的 硬件驱动库
    ```

---

## 🧪示例

### 示例：GPIO + Delay

闪烁点灯

```c++
using Led = gpio::PC13;

void Main()
{
    delay::init();
    while (true) {
        Led::toggle();
        delay::ms(500);
    }
}
```

### 示例：UART

非中断发送 `"hello world\r\n"`

```c++
using Com1 = uart::U1;

void Main()
{
    delay::init();
    while (true) {
        Com1::println("hello world");
        delay::s(1);
    }
}
```

中断发送 `"hello\r\n"`

```c++
using Com1 = uart::U1;

void Main()
{
    Com1::enable_irq_tx();

    while (true) {
    }
}

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
```

DMA 发送 `"hello world!\r\n"`

```c++
using Uart1TxDma = dma::Dma2S7;
using Com1       = uart::UartDma<1, Uart1TxDma, void>;
using Led  = gpio::PC13;

constexpr uint8_t      uart_index = 20;
EMPP_RAM_SRAM1 uint8_t uart_data[uart_index];

void Main()
{
    delay::init();
    constexpr char str[uart_index] = "hello world!\r\n";
    for (size_t i = 0; i < uart_index; i++)
        uart_data[i] = str[i];

    Com1::enable_dma_tx();
    Com1::config_dma_tx(reinterpret_cast<uint32_t>(uart_data), uart_index);
    Com1::enable_irq_dma_tx_tc();
    Com1::start_dma_tx();

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_tx();

            uart_flag = false;

            Led::toggle();
            delay::s(1);
            Com1::start_dma_tx();
        }
    }
}

void DMA2_Stream7_IRQHandler()
{
    if (Uart1TxDma::is_tc()) {
        uart_flag = true;
        Uart1TxDma::clear_tc();
    }
}
```

非中断接收 如果是 `'t'` 则反转 LED 电平

```c++
using Com1 = uart::U1;
using Led  = gpio::PC13;

void Main()
{
    while (true) {
        if (const auto r = Com1::read(); r == 't') {
            Led::toggle();
        }
    }
}
```

中断接收 如果是 `'t'` 则反转 LED 电平

```c++
using Com1 = uart::U1;
using Led  = gpio::PC13;


void Main()
{
    Com1::enable_irq_rx();

    while (true) {
        if (ch == 't') {
            Led::toggle();
            ch = 0;
        }
    }
}

static void callback_rx() { ch = Com1::read(); }

void USART1_IRQHandler()
{
    if (Com1::is_rc()) {
        callback_rx();
    }
}
```

DMA 接收定长数据

```c++
using Uart1RxDma = dma::Dma2S6;
using Com1       = uart::UartDma<1, void, Uart1RxDma>;
using Led  = gpio::PC13;

constexpr uint8_t      uart_index = 6;
EMPP_RAM_SRAM1 uint8_t uart_data[uart_index];

EMPP_RAM_ITCM void Main()
{
    Com1::enable_dma_rx();
    Com1::config_dma_rx(uart_data, uart_index);
    Com1::enable_irq_dma_rx_tc();
    Com1::start_dma_rx();

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_rx();
            uart_flag = false;

            Led::toggle();
            for (const auto i : uart_data)
                Com1::write(i);
            delay::s(1);

            Com1::start_dma_rx();
        }
    }
}

void DMA2_Stream6_IRQHandler()
{
    if (Uart1RxDma::is_tc()) {
        uart_flag = true;
        SCB_InvalidateDCache();
        Uart1RxDma::clear_tc();
    }
}
```

**更多例程** 👉 [example](https://github.com/mico845/empp/tree/main/doc/example)

## 📁 推荐目录结构

```
empp_pjt/                        
├─ empp/	         # EMPP 库
│ ├─ include/
│ └─ CMakeLists.txt
├─ stm32cubemx/      # CubeMX 生成
├─ cmake/            # CubeMX 生成
├─ Core/             # CubeMX 生成
├─ Drivers/          # CubeMX 生成
└─ UserApp/          # 用户程序
   ├─ common_inc.h   # C 和 C++ 混合
   ├─ Main.cpp       # CubeMX 生成的初始化代码之后跳转的 C++ 接口 Main() 
   ├─ Irq.cpp        # 中断服务函数
   ├─ board.hpp      # 针对 BSP 级进行命名空间划分          
   └─ CMakeLists.txt
```

---

## 📕 代码讲解

[【CSDN 使用现代 C++ 特性开发 STM32 的可能性测试】](https://blog.csdn.net/DreamTrue520/article/details/154950184?spm=1001.2014.3001.5502)

---

## 📜 License — Apache 2.0

```yaml
Apache License
Version 2.0, January 2004
```

本项目采用 Apache License 2.0，可安全用于商业项目、闭源产品和学术用途

---