# EMPP — Embedded Platform with C++

EMPP（Embedded Platform with C++） 是一款基于 STM32 的轻量级、追求高性能现代 C++ 嵌入式框架。

核心理念：

- 🧵 低抽象开销：尽可能让功能均可在编译期内联展开
- ⚡ 接近裸寄存器的执行性能
- 🧑‍💻 C++20 风格的嵌入式编程

目前支持：

| STM32  Series |                         |
|---------------|-------------------------|
| Peripheral    | H7xx (Test on H750VBT6) |
| `GPIO`        | ✅                       |
| `Delay`       | ✅                       |

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
    target_link_libraries(empp_platform PUBLIC stm32cubemx)
    ```

4. 引用 EMPP
    ```c++
    #include <empp.hpp>
    using namespace empp::stm32h7xx;
    ```

---

## 🧪示例

### 示例：GPIO + Delay

点灯

```c++
#include "common_inc.h"
using namespace empp::stm32h7xx;

using Led = gpio::C13;

void Main()
{
    delay::init();
    while (true) {
        Led::toggle();
        delay::ms(500);
    }
}
```

## 📁 推荐目录结构

```
empp_pjt/                        
├─ empp/	         # EMPP 库
│ ├─ include/
│ ├─ src/
│ └─ CMakeLists.txt
├─ stm32cubemx/      # CubeMX 生成
├─ cmake/            # CubeMX 生成
├─ Core/             # CubeMX 生成
├─ Drivers/          # CubeMX 生成
└─ UserApp/          # 用户程序
   ├─ common_inc.h   # C 和 C++ 混合
   ├─ Main.cpp           
   └─ CMakeLists.txt
```

---

## 🛣️ Roadmap（规划）

| 模块    | 状态  | 说明              |
|-------|-----|-----------------|
| Delay | 已完成 | 计划支持 DWT CYCCNT |
| UART  | 开发中 |                 |

剩下的随缘更新

---

## 📜 License — Apache 2.0

```yaml
Apache License
Version 2.0, January 2004
```

本项目采用 Apache License 2.0，可安全用于商业项目、闭源产品和学术用途

---