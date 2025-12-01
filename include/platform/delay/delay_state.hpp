// delay_state.hpp
#pragma once

#include "empp/type.hpp"

namespace empp::platform::delay {

struct DelayState
{
    static inline uint32_t ticks_per_us =
        0U; // 每 1us 需要多少个 CPU 周期（= sysclk MHz）
};

} // namespace empp::platform::delay
