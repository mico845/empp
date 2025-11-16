#pragma once

#include <cstdint>

#if defined(__GNUC__) || defined(__clang__)
    #define EMPP_ALWAYS_INLINE __attribute__((always_inline)) inline
#else
    #define EMPP_ALWAYS_INLINE inline
#endif
