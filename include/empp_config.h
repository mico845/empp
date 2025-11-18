// empp_config.h
#pragma once

// 选择 CHIP
#define EMPP_CHIP_STM32H7 1

// 主频 SYSCLK (uint: MHz)
#define EMPP_SYSCLK_MHZ (480)

// 选择 Delay 方式
#define EMPP_DELAY_USE_SYSTICK 0
#define EMPP_DELAY_USE_DWT     (!EMPP_DELAY_USE_SYSTICK)
