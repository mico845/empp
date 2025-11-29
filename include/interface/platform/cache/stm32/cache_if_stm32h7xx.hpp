// cache_if_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)
    #include "platform/cache/cache_impl.hpp"
    #include "platform/cache/backend/stm32/cache_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::cache {

using Impl = platform::cache::CacheImpl<CacheBackend>;

/* Op */
EMPP_STATIC_INLINE size_t line_size() EMPP_NOEXCEPT
{
    return Impl::line_size();
}

EMPP_STATIC_INLINE void invalidate_addr(const uintptr_t addr,
                                        const size_t    len) EMPP_NOEXCEPT
{
    Impl::invalidate_addr(addr, len);
}

EMPP_STATIC_INLINE void clean_addr(const uintptr_t addr,
                                   const size_t    len) EMPP_NOEXCEPT
{
    Impl::clean_addr(addr, len);
}

EMPP_STATIC_INLINE void clean_invalidate_addr(const uintptr_t addr,
                                              const size_t    len) EMPP_NOEXCEPT
{
    Impl::clean_invalidate_addr(addr, len);
}

EMPP_STATIC_INLINE void invalidate_ptr(const void  *ptr,
                                       const size_t len) EMPP_NOEXCEPT
{
    Impl::invalidate_ptr(ptr, len);
}

EMPP_STATIC_INLINE void clean_ptr(const void  *ptr,
                                  const size_t len) EMPP_NOEXCEPT
{
    Impl::clean_ptr(ptr, len);
}

EMPP_STATIC_INLINE void clean_invalidate_ptr(const void  *ptr,
                                             const size_t len) EMPP_NOEXCEPT
{
    Impl::clean_invalidate_ptr(ptr, len);
}

// obj
template <typename T>
EMPP_STATIC_INLINE void invalidate_obj(const T &obj) EMPP_NOEXCEPT
{
    Impl::invalidate_obj(obj);
}

template <typename T>
EMPP_STATIC_INLINE void clean_obj(const T &obj) EMPP_NOEXCEPT
{
    Impl::clean_obj(obj);
}

template <typename T>
EMPP_STATIC_INLINE void clean_invalidate_obj(const T &obj) EMPP_NOEXCEPT
{
    Impl::clean_invalidate_obj(obj);
}

// buf

template <typename T>
EMPP_STATIC_INLINE void invalidate_buf(const T     *buf,
                                       const size_t count) EMPP_NOEXCEPT
{
    Impl::invalidate_buf(buf, count);
}

template <typename T>
EMPP_STATIC_INLINE void clean_buf(const T     *buf,
                                  const size_t count) EMPP_NOEXCEPT
{
    Impl::clean_buf(buf, count);
}

template <typename T>
EMPP_STATIC_INLINE void clean_invalidate_buf(const T     *buf,
                                             const size_t count) EMPP_NOEXCEPT
{
    Impl::clean_invalidate_buf(buf, count);
}

} // namespace empp::stm32h7xx::cache
#endif