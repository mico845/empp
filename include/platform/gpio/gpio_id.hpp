// gpio_id.hpp
#pragma once
#include "empp/driver.hpp"

namespace empp::platform::gpio {

struct PinId
{
    uint8_t port; // 'A' → 0, 'B' → 1, ...
    uint8_t pin;  // 0~15

    constexpr bool operator==(const PinId &) const = default;
};

} // namespace empp::platform::gpio
