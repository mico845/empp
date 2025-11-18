# empp CMakeLists
cmake_minimum_required(VERSION 3.22)

add_library(empp_platform STATIC)

# 头文件
target_include_directories(empp_platform PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# C++20
target_compile_features(empp_platform PUBLIC cxx_std_20)

# 源文件
target_sources(empp_platform PRIVATE
        src/interface/stm32/uart_irq_dispatcher_stm32h7xx.cpp
)