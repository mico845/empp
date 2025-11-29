// cache_backend_stm32h7xx.hpp

#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "empp/driver.hpp"
    #include "empp/define.hpp"
    #include "empp/type.hpp"

namespace empp::stm32h7xx::cache {

class CacheBackend
{
public:
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
    static constexpr size_t DCACHE_LINE_SIZE = 32U;
    #endif

    EMPP_STATIC_INLINE size_t line_size() EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        return DCACHE_LINE_SIZE;
    #else
        return 0U;
    #endif
    }

    EMPP_STATIC_INLINE void invalidate_addr(const uintptr_t addr,
                                            const size_t    len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        if ((addr == 0U) || (len == 0U)) {
            return;
        }

        const uintptr_t aligned = align_addr_to_line(addr);
        const size_t    offset  = addr - aligned;
        size_t          size    = len + offset;

        size = align_size_to_line(size);

        SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t *>(aligned),
                                     static_cast<int32_t>(size));
    #else
        (void)addr;
        (void)len;
    #endif
    }

    EMPP_STATIC_INLINE void clean_addr(const uintptr_t addr,
                                       const size_t    len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        if ((addr == 0U) || (len == 0U)) {
            return;
        }

        const auto   aligned = align_addr_to_line(addr);
        const size_t offset  = addr - aligned;
        size_t       size    = len + offset;

        size = align_size_to_line(size);

        SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t *>(aligned),
                                static_cast<int32_t>(size));
    #else
        (void)addr;
        (void)len;
    #endif
    }

    EMPP_STATIC_INLINE void
    clean_invalidate_addr(const uintptr_t addr, const size_t len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        if ((addr == 0U) || (len == 0U)) {
            return;
        }

        const uintptr_t aligned = align_addr_to_line(addr);
        const size_t    offset  = addr - aligned;
        size_t          size    = len + offset;

        size = align_size_to_line(size);

        SCB_CleanInvalidateDCache_by_Addr(reinterpret_cast<uint32_t *>(aligned),
                                          static_cast<int32_t>(size));
    #else
        (void)addr;
        (void)len;
    #endif
    }

private:
    // 对地址向下对齐到 Cache Line
    EMPP_STATIC_INLINE constexpr uintptr_t
    align_addr_to_line(const uintptr_t addr) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        return addr & ~(static_cast<uintptr_t>(DCACHE_LINE_SIZE) - 1U);
    #else
        (void)addr;
        return 0U;
    #endif
    }

    // 将 size 向上补齐到 Cache Line 的整数倍
    EMPP_STATIC_INLINE constexpr size_t
    align_size_to_line(const size_t size) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) \
        && (EMPP_USE_CACHE == 1U)
        return (size + DCACHE_LINE_SIZE - 1U)
               & ~(static_cast<size_t>(DCACHE_LINE_SIZE) - 1U);
    #else
        (void)size;
        return 0U;
    #endif
    }
};

} // namespace empp::stm32h7xx::cache

#endif
