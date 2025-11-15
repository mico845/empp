#pragma once

// 选择 CHIP
#define EMPP_CHIP_STM32H7 1

// 选择 Delay 方式
#define EMPP_DELAY_USE_SYSTICK 1
#define EMPP_DELAY_USE_DWT     (!EMPP_DELAY_USE_SYSTICK)
