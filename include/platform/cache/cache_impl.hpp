// cache_impl.hpp
#pragma once

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
        Backend::invalidate_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_addr(uintptr_t addr, size_t len) EMPP_NOEXCEPT
    {
        Backend::clean_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_invalidate_addr(uintptr_t addr,
                                                  size_t    len) EMPP_NOEXCEPT
    {
        Backend::clean_invalidate_addr(addr, len);
    }

    EMPP_STATIC_INLINE void invalidate_ptr(const void *ptr,
                                           size_t      len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        Backend::invalidate_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_ptr(const void *ptr, size_t len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        Backend::clean_addr(addr, len);
    }

    EMPP_STATIC_INLINE void clean_invalidate_ptr(const void *ptr,
                                                 size_t      len) EMPP_NOEXCEPT
    {
        const auto addr = reinterpret_cast<uintptr_t>(ptr);
        Backend::clean_invalidate_addr(addr, len);
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
