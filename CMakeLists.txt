# empp CMakeLists
cmake_minimum_required(VERSION 3.22)

add_library(empp_platform INTERFACE)

target_include_directories(empp_platform INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_features(empp_platform INTERFACE
        cxx_std_20
)