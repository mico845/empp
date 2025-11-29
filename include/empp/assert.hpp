// assert.hpp
#pragma once

#include "empp_config.h"

#if !defined(EMPP_ASSERT)
    #if defined(EMPP_USE_ASSERT) && (EMPP_USE_ASSERT == 1U)
        #define EMPP_ASSERT(cond, msg)                                      \
            do {                                                       \
                if (!(cond)) {                                         \
                    (void)msg;                                    \
                }                                                      \
            } while (0)
    #else
        #define EMPP_ASSERT(cond, msg)
    #endif
#endif
