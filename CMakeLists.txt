cmake_minimum_required(VERSION 3.22)

add_library(empp_platform)

target_sources(empp_platform PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/gpio/gpio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/delay/delay_systick.cpp
)

target_include_directories(empp_platform PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


