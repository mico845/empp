# EMPP — Embedded Platform with C++

EMPP（Embedded Platform with C++） 是一款基于 STM32 **追求最高性能和最低开销** 的 现代 C++ 轻量级嵌入式框架。

<p align="center">
  <img src="https://raw.githubusercontent.com/mico845/empp/main/doc/icon/empp.png" width="280">
</p>

核心理念：

- 🧵 低抽象开销：尽可能所有功能均可在编译期内联展开
- ⚡ 接近裸寄存器的执行性能
- 🧑‍💻 C++20 风格的嵌入式编程
- 📕 Only-Header 仅头文件库，轻量

## 目前支持

### 外设驱动

| platform | H7xx (Test on H750VBT6) | G4xx |
|----------|-------------------------|------|
| `gpio`   | ✅                       | ?    |
| `delay`  | ✅                       | ?    |
| `uart`   | ✅                       | ?    |
| `dma`    | ✅                       | ?    |
| `cache`  | ✅                       | ?    |
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

**这里均省略了 STM32CubeMX 的初始化代码**

**假设已经正确配置好时钟和外设**

### 示例：点灯程序

```c++
using Led  = gpio::PC13;         
using Com1 = uart::U1;

delay::init();                  // 👈 初始化延时（DWT / SysTick）

Com1::println("LED blink start");

while (true) {
  Led::toggle();              // 👈 翻转 LED 状态
  delay::ms(500);             // 👈 延时 500ms
}
```

### 示例：GPIO 操作

输出高低电平、反转电平、读取电平状态：

```c++
using namespace empp::stm32h7xx::gpio;

PC0::reset();                   // 👈 输出低电平
PC0::set();                     // 👈 输出高电平
PC0::toggle();                  // 👈 反转一次电平
const bool level = PC1::read(); // 👈 读取当前电平状态
```

### 示例：Delay 阻塞延时

阻塞延时 100us / 10ms / 1s

```c++
// - 若 EMPP_DELAY_USE_DWT = 1, EMPP_DELAY_USE_SYSTICK = 0, 则使用 DWT
// 作延时；
// - 若 EMPP_DELAY_USE_SYSTICK = 1, EMPP_DELAY_USE_DWT = 0, 则使用 SysTick
// 作延时；
// - 内部会根据 EMPP_SYSCLK_MHZ 设置 ticks_per_us。
delay::init(); // 👈 初始化延时（DWT / SysTick）



delay::us(100);   // 👈 延时 100 微秒
delay::ms(10);    // 👈 延时 10 毫秒
delay::s(1);      // 👈 延时 1 秒（内部为 1000 * ms(1)）
```

### 示例：DWT 测量运行时间

使用 DWT 测量代码运行时间

```c++
delay::init();                              // 👈 初始化 (务必使用 DWT 即 DWT EMPP_DELAY_USE_DWT = 1)

const uint32_t time_us = delay::measure_us([] {
  /* 测量运行开始时间 👈 */
  gpio::PD12::toggle();
  delay::us(1);
  gpio::PD12::toggle();
  /* 测量运行结束时间 👈 */
});

Com1::println("time:", time_us, "us");      // 👈 打印测量结果 time_us
```

### 示例：UART 阻塞发送

发送字符串、拼接字符串、无符号类型、有符号类型，逐 Byte 发送

```c++
using Com1 = uart::U1;

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
```

### 示例：UART 轮询接收

轮询接收——如果接收为 `'t'` 则反转 LED 电平

```c++
using Com1 = uart::U1;
using Led  = gpio::PC13;

while (true) {
  if (const auto r = Com1::read(); r == 't') {    /* 👈 Com1::read() 读取一个 uint8_t */
      Led::toggle();
  }
}
```

### 示例：UART TX 中断发送

中断发送 `"hello\r\n"`

```c++
using Com1 = uart::U1;

void Main()
{
    Com1::enable_irq_tx();   // 👈 使能 UART tx 中断

    for (;;) {
        __NOP();
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
    if (Com1::is_tc()) {    // 👈 检测是否是 tx 完成中断
        callback_tx();      // 跳转 callback_tx 执行
    }
}
```

### 示例：UART RX 中断读取

中断读取——如果是 `'t'` 则反转 LED 电平

```c++
using Com1 = uart::U1;
using Led  = gpio::PC13;

inline volatile uint8_t ch = 0;

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

static void callback_rx() { ch = Com1::read(); }

void USART1_IRQHandler()
{
    if (Com1::is_rx()) { // 👈 检测是否是 rx 完成中断
        callback_rx();   // 跳转 callback_rx 执行
    }
}
```

### 示例：UART TX DMA 发送

DMA 发送 `"[empp]:hello world!\r\n"` + DMA 传输完成中断

```c++
/*
 * Uart1 TX DMA (DMA2 Stream7)
 * Mode: Normal
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1TxDma = dma::Dma2S7;

using Com1 = uart::Uart<1, Uart1TxDma, void>;
using Led  = gpio::PC13;

inline volatile bool uart_flag = false;

constexpr uint8_t      UART_TX_BYTES            = 24;
EMPP_RAM_SRAM1 uint8_t uart_data[UART_TX_BYTES] = {};   /* Write through, read allocate，no write allocate */

void Main()
{
    delay::init();
    constexpr char str[UART_TX_BYTES] = "[empp]:hello world!\r\n";

    for (size_t i = 0; i < UART_TX_BYTES; i++)
        uart_data[i] = str[i];

    Com1::enable_dma_tx();                              // 👈 允许 UART 通过 DMA 发送数据
    Com1::config_dma_tx(uart_data, UART_TX_BYTES);      // 👈 配置 DMA 传输地址与长度
    Com1::enable_irq_dma_tx_tc();                       // 👈 使能 DMA TX 传输完成中断
    Com1::start_dma_tx();                               // 👈 启动 TX 方向 DMA 传输

    for (;;) {
        if (uart_flag) {
            Com1::stop_dma_tx();                        // 👈 停止 TX 方向 DMA 传输

            uart_flag = false;

            Led::toggle();
            delay::s(1);
            Com1::start_dma_tx();
        }
    }
}

void DMA2_Stream7_IRQHandler()
{
    if (Uart1TxDma::is_tc()) {      // 👈 传输完成中断
        uart_flag = true;
        Uart1TxDma::clear_tc();     // 👈 手动清除传输完成标志位
    }
}
```

DMA 发送 `"[empp]:hello world!\r\n"` + Ring FIFO 环形缓存区

```c++
/*
 * Uart1 TX DMA (DMA2 Stream7)
 * Mode: Normal
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1TxDma = dma::Dma2S7;
using Com1 = uart::Uart<1, Uart1TxDma, void>;

constexpr uint8_t UART_TXFIFO_BYTES = 128;
EMPP_RAM_AXI_SRAM empp::fifo<uint8_t, UART_TXFIFO_BYTES> uart_fifo; /* Write back, Read allocate，Write allocate */

static void send_data()
{
    size_t len = 0;
    while ((len = uart_fifo.linear_read_length()) > 0)  /* 👈 FIFO 获取线性可读数据长度 */
    {
        const auto data =
            uart_fifo.linear_read_ptr();                // 👈 FIFO 获取线性可读数据指针

        Com1::config_dma_tx(data, len);                 // 👈 配置 DMA 传输地址与长度
        Com1::start_dma_tx();                           // 👈 启动 TX 方向 DMA 传输

        uart_fifo.skip(len);                            // 👈 FIFO 跳过已读数据
    }
}

void Main()
{
    constexpr uint8_t str[] = "[empp]:hello world!\r\n";

    uart_fifo.write(str, sizeof(str));
    cache::clean_obj<empp::fifo<uint8_t, UART_TXFIFO_BYTES>>(uart_fifo); // 👈 clean DCache

    Com1::enable_dma_tx(); // 👈 允许 UART 通过 DMA 发送数据
    
    send_data();

    for (;;) {
        __NOP();
    }
}
```

### 示例：UART RX DMA 接收

DMA 接收定长数据 + DMA 传输完成中断

```c++
/*
 * Uart1 RX DMA (DMA2 Stream6)
 * Mode: Normal
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1RxDma = dma::Dma2S6;

using Com1 = uart::Uart<1, void, Uart1RxDma>;
using Led  = gpio::PC13;

inline volatile bool uart_flag = false;

constexpr uint8_t UART_RX_BYTES = 6;

EMPP_RAM_SRAM1 inline uint8_t uart_data[UART_RX_BYTES] =
    {}; /* Write through, read allocate，no write allocate */

void Main()
{
    Com1::enable_dma_rx();                                  // 👈 允许 UART 通过 DMA 接收数据
    Com1::config_dma_rx(uart_data, UART_RX_BYTES);          // 👈 配置 DMA 传输地址与长度
    Com1::enable_irq_dma_rx_tc();                           // 👈 使能 DMA RX 传输完成中断
    Com1::start_dma_rx();                                   // 👈 启动 RX 方向 DMA 传输

    while (true) {
        if (uart_flag) {
            Com1::stop_dma_rx();                            // 👈 停止 RX 方向 DMA 传输
            uart_flag = false;

            Led::toggle();
            Com1::print(uart_data, UART_RX_BYTES);
            delay::s(1);

            Com1::start_dma_rx();
        }
    }
}

void DMA2_Stream6_IRQHandler()
{
    if (Uart1RxDma::is_tc()) {                              // 👈 传输完成中断
        uart_flag = true;
        cache::invalidate_buf(uart_data, UART_RX_BYTES);    // 👈 invalidate Cache，保证读取到最新数据
        Uart1RxDma::clear_tc();                             // 👈 手动清除传输完成标志位
    }
}
```

DMA 接收不定长数据（DMA传输半完成中断 + DMA传输完成中断 + IDLE 空闲中断）

```c++
/*
 * Uart1 RX DMA (DMA2 Stream6)
 * Mode: Circular
 * Fifo: Disable
 * DataWidth: P: Byte M: Byte
 */
using Uart1RxDma = dma::Dma2S6;

using Com1 = uart::Uart<1, void, Uart1RxDma>;
using Led  = gpio::PC13;

inline constexpr uint16_t UART_DMA_RX_BUFFER_SIZE = 8;
inline constexpr uint16_t UART_DMA_RX_FIFO_SIZE   = 128;

/* Write through, read allocate，no write allocate */
EMPP_RAM_SRAM1 inline uint8_t bufferRxDma[UART_DMA_RX_BUFFER_SIZE] = {};

inline empp::fifo<uint8_t, UART_DMA_RX_FIFO_SIZE> fifoRxDma; // 可选：用环形缓冲区封装;

// 有新数据到来（HT/TC/IDLE 任一触发）
inline volatile bool g_rx_data_ready = false;

// 一帧的结束（触发过 IDLE）
inline volatile bool g_rx_idle_event = false;

uint8_t fifo_buf[UART_DMA_RX_FIFO_SIZE];

static void process_rx_data() noexcept
{

    const auto len = fifoRxDma.available_read();
    if (len == 0U) {
        return;
    }

    fifoRxDma.read(fifo_buf, len);
    Com1::print("\r\n$[empp Rx]:");
    Com1::print(fifo_buf, len);
    Com1::print("\r\n");
}

EMPP_RAM_ITCM void Main()
{
    delay::init();

    Com1::enable_dma_rx();
    Com1::enable_irq_dma_rx_ht(); // 👈 使能 DMA RX 传输半完成中断
    Com1::enable_irq_dma_rx_tc(); // 👈 使能 DMA RX 传输完成中断

    Com1::config_dma_rx(bufferRxDma, UART_DMA_RX_BUFFER_SIZE);
    Com1::start_dma_rx();

    delay::ms(20);
    Com1::clear_idle();      // 👈 UART初始化后会产生空闲帧，延时后再清除IDLE标志（可选）
    Com1::enable_irq_idle(); // 👈 开 RX IDLE 中断

    /*
     * - 数据量未达到半满，触发空闲中断
     * - 数据量达到半满，未达到满溢，先触发半满中断，后触发空闲中断
     * - 数据量刚好达到满溢，先触发半满中断，后触发满溢中断
     * - 数据量大于缓冲区长度，DMA循环覆盖溢出的字节
     */

    for (;;) {
        if (g_rx_idle_event) {
            g_rx_data_ready = false;
            g_rx_idle_event = false;

            process_rx_data(); // 处理这一次“帧结束”之前的全部数据
        }

        // 预留扩展点：如果未来希望“流式处理”，可以在此使用 g_rx_data_ready
        if (g_rx_data_ready) {
            /* something */
        }
    }
}

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
```

### 更多示例

👉 [empp/doc/example](https://github.com/mico845/empp/tree/main/doc/example)

---

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