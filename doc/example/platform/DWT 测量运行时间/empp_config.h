// empp_config.h
#pragma once

// 选择 CHIP
#define EMPP_CHIP_STM32H7 1U

// 主频 SYSCLK (uint: MHz)
#if !defined(EMPP_SYSCLK_MHZ)
    #define EMPP_SYSCLK_MHZ (480)
#endif

// 选择 Delay 方式
#if !defined(EMPP_DELAY_USE_SYSTICK) || !defined(EMPP_DELAY_USE_DWT)
    #define EMPP_DELAY_USE_SYSTICK 0U   // 👈 目前仅 DWT 支持测量运行时间功能
    #define EMPP_DELAY_USE_DWT     (!EMPP_DELAY_USE_SYSTICK)
#endif

// 是否使用 EMPP 内置的 Component 驱动
#if !defined(EMPP_USE_COMPONENTS)
    #define EMPP_USE_COMPONENTS 1U
#endif

// DEBUG 关闭所有内联 进行单步调试
#if !defined(EMPP_DEBUG_ONESTEP)
    #define EMPP_DEBUG_ONESTEP 0U
#endif

#if !defined(EMPP_USE_CACHE)
    #define EMPP_USE_CACHE 1U
#endif
