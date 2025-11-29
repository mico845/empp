// type.hpp
#pragma once

#include <cstdint>
#include <concepts>

#include "empp/define.hpp"
#include "empp/fifo.hpp"

namespace empp {

using Callback = void (*)();

using size_t    = std::size_t;
using uintptr_t = std::uintptr_t;

using NullPtr = std::nullptr_t;

} // namespace empp