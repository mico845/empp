# empp CMakeLists
cmake_minimum_required(VERSION 3.22)

add_library(empp_platform INTERFACE)

# 头文件
target_include_directories(empp_platform INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# C++20
target_compile_features(empp_platform INTERFACE cxx_std_20)


# 禁用 LwRB 的 C11 atomic
target_compile_definitions(empp_platform INTERFACE LWRB_DISABLE_ATOMIC=1)

add_subdirectory(thirdparty/lwrb/lwrb)

target_link_libraries(empp_platform INTERFACE lwrb)
