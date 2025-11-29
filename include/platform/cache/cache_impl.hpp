// cache_impl.hpp
#pragma once
#include "empp_config.h"
#include "empp/assert.hpp"
#include "empp/define.hpp"
#include "empp/type.hpp"
#include "cache_concept.hpp"

namespace empp::platform::cache {

template <CacheBackend Backend>
struct CacheImpl
{
    /* Op */
    EMPP_STATIC_INLINE size_t line_size() EMPP_NOEXCEPT
    {
        return Backend::line_size();
    }

    EMPP_STATIC_INLINE void invalidate_addr(uintptr_t addr,
                                            size_t    len) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(addr != 0U, "Cache invalidate_addr: addr must not be 0");
        EMPP_ASSERT(len != 0U, "Cache invalidate_addr: len must not be 0");
#endif

#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        // CMSIS SCB_*DCache_by_Addr 第二参数是 int32_t（字节数）
        using int_bytes_t = int32_t;
        constexpr auto max_len =
            static_cast<size_t>(std::numeric_limits<int_bytes_t>::max());

        EMPP_ASSERT((len <= max_len),
                    "STM32H7 DCache operation length exceeds int32_t range");
#endif

#if defined(EMPP_USE_CACHE) && (EMPP_USE_CACHE == 1U)
        Backend::invalidate_addr(addr, len);
#else
        (void)addr;
        (void)len;
#endif
    }

    EMPP_STATIC_INLINE void clean_addr(uintptr_t addr, size_t len) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(addr != 0U, "Cache clean_addr: addr must not be 0");
        EMPP_ASSERT(len != 0U, "Cache clean_addr: len must not be 0");
#endif
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        // CMSIS SCB_*DCache_by_Addr 第二参数是 int32_t（字节数）
        using int_bytes_t = int32_t;
        constexpr auto max_len =
            static_cast<size_t>(std::numeric_limits<int_bytes_t>::max());

        EMPP_ASSERT((len <= max_len),
                    "STM32H7 DCache operation length exceeds int32_t range");
#endif

#if defined(EMPP_USE_CACHE) && (EMPP_USE_CACHE == 1U)
        Backend::clean_addr(addr, len);
#else
        (void)addr;
        (void)len;
#endif
    }

    EMPP_STATIC_INLINE void clean_invalidate_addr(uintptr_t addr,
                                                  size_t    len) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U)
        EMPP_ASSERT(addr != 0U,
                    "Cache clean_invalidate_addr: addr must not be 0");
        EMPP_ASSERT(len != 0U,
                    "Cache clean_invalidate_addr: len must not be 0");
#endif
#if defined(EMPP_DEBUG_CHECK) && (EMPP_DEBUG_CHECK == 1U) \
    && defined(EMPP_CHIP_STM32H7)
        // CMSIS SCB_*DCache_by_Addr 第二参数是 int32_t（字节数）
        using int_bytes_t = int32_t;
        constexpr auto max_len =
            static_cast<size_t>(std::numeric_limits<int_bytes_t>::max());

        EMPP_ASSERT((len <= max_len),
                    "STM32H7 DCache operation length exceeds int32_t range");
#endif

#if defined(EMPP_USE_CACHE) && (EMPP_USE_CACHE == 1U)
        Backend::clean_invalidate_addr(addr, len);
#else
        (void)addr;
        (void)len;
#endif
    }

    EMPP_STATIC_INLINE void invalidate_ptr(const void *ptr,
                                           size_t      len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        invalidate_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_ptr(const void *ptr, size_t len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        clean_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_invalidate_ptr(const void *ptr,
                                                 size_t      len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        clean_invalidate_addr(addr, len);
    }

    // obj

    template <typename T>
    EMPP_STATIC_INLINE void invalidate_obj(const T &obj) EMPP_NOEXCEPT
    {
        invalidate_ptr(&obj, sizeof(T));
    }

    template <typename T>
    EMPP_STATIC_INLINE void clean_obj(const T &obj) EMPP_NOEXCEPT
    {
        clean_ptr(&obj, sizeof(T));
    }

    template <typename T>
    EMPP_STATIC_INLINE void clean_invalidate_obj(const T &obj) EMPP_NOEXCEPT
    {
        clean_invalidate_ptr(&obj, sizeof(T));
    }

    // buf

    template <typename T>
    EMPP_STATIC_INLINE void invalidate_buf(const T     *buf,
                                           const size_t count) EMPP_NOEXCEPT
    {
        invalidate_ptr(buf, sizeof(T) * count);
    }

    template <typename T>
    EMPP_STATIC_INLINE void clean_buf(const T     *buf,
                                      const size_t count) EMPP_NOEXCEPT
    {
        clean_ptr(buf, sizeof(T) * count);
    }

    template <typename T>
    EMPP_STATIC_INLINE void
    clean_invalidate_buf(const T *buf, const size_t count) EMPP_NOEXCEPT
    {
        clean_invalidate_ptr(buf, sizeof(T) * count);
    }
};

} // namespace empp::platform::cache
