#pragma once
#include "empp/driver.hpp"

namespace empp {

namespace platform::rcc {
inline uint32_t EMPP_RCC_GetSystemClockFreq()
{
    uint32_t             frequency = 0U;
    LL_PLL_ClocksTypeDef PLL_Clocks;

    /* Get SYSCLK source
     * -------------------------------------------------------*/
    switch (LL_RCC_GetSysClkSource()) {
        /* No check on Ready: Won't be selected by hardware if not */
        case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:
            frequency =
                HSI_VALUE >> (LL_RCC_HSI_GetDivider() >> RCC_CR_HSIDIV_Pos);
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_CSI:
            frequency = CSI_VALUE;
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:
            frequency = HSE_VALUE;
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_PLL1:
            LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
            frequency = PLL_Clocks.PLL_P_Frequency;
            break;

        default:
            /* Nothing to do */
            break;
    }

    return frequency;
}
} // namespace platform::rcc

namespace stm32h7::rcc {
inline uint32_t EMPP_RCC_GetSystemClockFreq()
{
    return platform::rcc::EMPP_RCC_GetSystemClockFreq();
}
} // namespace stm32h7::rcc

} // namespace empp