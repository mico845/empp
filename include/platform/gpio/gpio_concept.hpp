// gpio_concept.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::gpio {

template <typename T>
concept GpioBackend = std::is_empty_v<T> // 保证 0 开销
                      && requires {
                             {
                                 T::set()
                             }
                             EMPP_NOEXCEPT;
                             {
                                 T::reset()
                             }
                             EMPP_NOEXCEPT;
                             {
                                 T::toggle()
                             }
                             EMPP_NOEXCEPT;
                             {
                                 T::read()
                             }
                             EMPP_NOEXCEPT->std::same_as<bool>;
                         };

} // namespace empp::platform::gpio
