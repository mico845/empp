// define.hpp
#pragma once
#include "empp_config.h"

#if EMPP_DEBUG_ONESTEP
    #define EMPP_ALWAYS_INLINE
#else
    #if defined(__GNUC__) || defined(__clang__)
        #define EMPP_ALWAYS_INLINE __attribute__((always_inline)) inline
    #else
        #define EMPP_ALWAYS_INLINE inline
    #endif
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define EMPP_WEAK __attribute__((weak))
#else
    #define EMPP_WEAK __attribute__((weak))
#endif

// =================== 内存区域选择宏 ===================
#if defined(EMPP_CHIP_STM32H7)
    #if defined(__GNUC__) || defined(__clang__)
        /*
         * DTCM 用于数据存取，特点是跟内核速度一样
         */
        #define EMPP_RAM_DTCM __attribute__((section(".dtcm")))

        /*
         * AXI SRAM
         * 位于D1域，数据带宽是64bit，挂在AXI总线上。除了D3域中的BDMB主控不能访问，其它都可以访问此RAM区。
         */
        #define EMPP_RAM_AXI_SRAM __attribute__((section(".axi_sram")))

        /*
         * SRAM1、SRAM2、SRAM3
         * 位于D2域，数据带宽是32bit，挂在AHB总线上。除了D3域中的BDMB主控不能访问这三块SRAM，其它都可以访问这几个RAM区。
         */
        #define EMPP_RAM_SRAM1 __attribute__((section(".sram1")))
        #define EMPP_RAM_SRAM2 __attribute__((section(".sram2")))
        #define EMPP_RAM_SRAM3 __attribute__((section(".sram3")))
        /*
         * SRAM4
         * 位于D3域，数据带宽是32bit，挂在AHB总线上，大部分主控都能访这块SRAM区。
         */
        #define EMPP_RAM_SRAM4 __attribute__((section(".sram4")))

        /*
         * ITCM 指令加速 （在移植时请参考 ld 脚本和 startup_*.s 启动汇编）
         */
        #define EMPP_RAM_ITCM __attribute__((section(".itcm")))
    #else

    #endif
#endif
