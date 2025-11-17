cmake_minimum_required(VERSION 3.22)

add_library(empp_platform)

target_sources(empp_platform PRIVATE
        src/platform/delay/delay_systick_backend_stm32h7xx.cpp
        src/platform/delay/delay_dwt_backend_stm32h7xx.cpp
)

target_include_directories(empp_platform PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


