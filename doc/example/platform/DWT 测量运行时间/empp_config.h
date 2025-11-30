// empp_config.h
#pragma once

// 选择 CHIP
#define EMPP_CHIP_STM32H7 1U

// 是否启用 Cache
#if !defined(EMPP_USE_CACHE)
    #define EMPP_USE_CACHE 1U
#endif

// 主频 SYSCLK (uint: MHz)
#if !defined(EMPP_SYSCLK_MHZ)
    #define EMPP_SYSCLK_MHZ (480)
#endif

// 选择 Delay 方式
// 推荐 DWT （精度更高且支持时间测量）
// DWT 设置 EMPP_DELAY_USE_SYSTICK = 0U
#if !defined(EMPP_DELAY_USE_SYSTICK) && !defined(EMPP_DELAY_USE_DWT)
    #define EMPP_DELAY_USE_SYSTICK 0U   // 👈 目前仅 DWT 支持测量运行时间功能
    #define EMPP_DELAY_USE_DWT     (!EMPP_DELAY_USE_SYSTICK)
#endif

// 是否使用 EMPP 内置的 Component 驱动
#if !defined(EMPP_USE_COMPONENTS)
    #define EMPP_USE_COMPONENTS 1U
#endif

// Clion 的 DEBUG 模式下不定义 NDEBUG
#if !defined(NDEBUG)

    // DEBUG 关闭所有内联 进行单步调试
    #if !defined(EMPP_DEBUG_ONESTEP)
        #define EMPP_DEBUG_ONESTEP 1U
    #endif

    // DEBUG 类型检查
    #if !defined(EMPP_DEBUG_CHECK)
        #define EMPP_DEBUG_CHECK 1U
    #endif

    // 使能断言
    #if !defined(EMPP_USE_ASSERT)
        #define EMPP_USE_ASSERT 1U
    #endif

#endif
