// cache_concept.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::cache {

template <typename T>
concept CacheBackend = requires(uintptr_t addr, size_t len) {
    {
        T::line_size()
    }
    EMPP_NOEXCEPT->std::same_as<size_t>;
    {
        T::invalidate_addr(addr, len)
    }
    EMPP_NOEXCEPT;
    {
        T::clean_addr(addr, len)
    }
    EMPP_NOEXCEPT;
    {
        T::clean_invalidate_addr(addr, len)
    }
    EMPP_NOEXCEPT;
};

} // namespace empp::platform::cache