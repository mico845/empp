// board.hpp
#pragma once

#include "empp.h"

namespace board {

using namespace empp::stm32h7xx;
using namespace empp::components;

using Com1       = uart::U1;
using Led        = gpio::PC13;
} // namespace board