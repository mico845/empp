#pragma once

#include <cstdint>
#include <concepts>

#if defined(__GNUC__) || defined(__clang__)
    #define EMPP_ALWAYS_INLINE __attribute__((always_inline)) inline
#else
    #define EMPP_ALWAYS_INLINE inline
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define EMPP_WEAK __attribute__((weak))
#else
    #define EMPP_ALWAYS_INLINE __attribute__((weak))
#endif


namespace empp {

using Callback = void (*)();

}