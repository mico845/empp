#pragma once
#include "empp/type.hpp"
#include "empp_config.hpp"

#if EMPP_DELAY_USE_SYSTICK
    #include "delay_systick.hpp"
#elif EMPP_DELAY_USE_DWT
    #include "delay_dwt.hpp"
#endif

namespace empp::stm32h7::delay {

inline void init()
{
#if EMPP_DELAY_USE_SYSTICK
    platform::delay::systick::init();
#elif EMPP_DELAY_USE_DWT

#endif
}

inline void init(const uint16_t sysclk_mhz)
{
#if EMPP_DELAY_USE_SYSTICK
    platform::delay::systick::init(sysclk_mhz);
#elif EMPP_DELAY_USE_DWT
#endif
}

inline void ms(uint16_t nMs)
{
#if EMPP_DELAY_USE_SYSTICK
    platform::delay::systick::ms(nMs);
#elif EMPP_DELAY_USE_DWT
#endif
}
inline void us(uint32_t nUs)
{
#if EMPP_DELAY_USE_SYSTICK
    platform::delay::systick::us(nUs);
#elif EMPP_DELAY_USE_DWT
#endif
}

} // namespace empp::stm32h7::delay
