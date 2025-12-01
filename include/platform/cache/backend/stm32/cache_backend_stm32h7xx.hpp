// cache_backend_stm32h7xx.hpp

#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "empp/driver.hpp"
    #include "empp/define.hpp"
    #include "empp/type.hpp"

namespace empp::stm32h7xx::cache {
/**
 * @brief STM32H7 D-Cache Backend
 *
 * @details
 * 该后端封装了与 STM32H7 D-Cache 相关的基础操作，主要用于：
 * - 根据地址和长度，按 Cache Line 对齐进行 clean / invalidate；
 * - 在无 D-Cache 的配置下自动退化为空操作（参数被静默丢弃）；
 * - 通过 @ref line_size() 暴露 D-Cache 行大小（字节）。
 */
class CacheBackend
{
public:
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    /// @brief STM32H7 D-Cache 行大小（单位：字节）。
    static constexpr size_t DCACHE_LINE_SIZE = 32U;
    #endif

    /**
     * @brief 返回当前 D-Cache 行大小。
     *
     * @return
     * - 若芯片存在 D-Cache，则返回行大小（例如 H7 通常为 32 字节）；
     * - 若无 D-Cache，则返回 0。
     *
     * @note
     * 可用于上层在编译期/运行期决定是否做 Cache 对齐操作。
     */
    [[nodiscard]] EMPP_STATIC_INLINE size_t line_size() EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
        return DCACHE_LINE_SIZE;
    #else
        return 0U;
    #endif
    }

    /**
     * @brief 对指定内存区域执行 D-Cache 失效 (invalidate) 操作。
     *
     * @param addr 起始地址（任意对齐度）。
     * @param len  区域长度（字节数）。
     *
     * @details
     * - 内部会将 @p addr 向下对齐到 Cache Line 边界；
     * - 并将 @p len 向上补齐到 Cache Line 的整数倍；
     * - 然后调用 SCB_InvalidateDCache_by_Addr() 进行区域性失效。
     *
     * 典型使用场景：
     * - 外设通过 DMA 向内存写入数据后，在 CPU 读取之前调用，
     *   保证 CPU 不再使用旧的 Cache 内容，而是重新从内存取数。
     *
     * 在无 D-Cache 的配置下，该函数为完全空操作。
     */
    EMPP_STATIC_INLINE void invalidate_addr(const uintptr_t addr,
                                            const size_t    len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
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

    /**
     * @brief 对指定内存区域执行 D-Cache 清除 (clean) 操作。
     *
     * @param addr 起始地址（任意对齐度）。
     * @param len  区域长度（字节数）。
     *
     * @details
     * - 内部同样会进行地址向下对齐、长度向上补齐；
     * - 然后调用 SCB_CleanDCache_by_Addr() 将 Cache 中脏数据写回内存。
     *
     * 典型使用场景：
     * - CPU 向某个缓冲区写入数据后，交给 DMA 读取发送；
     * - 在启动 DMA 之前调用 clean，保证内存中数据是最新的。
     *
     * 在无 D-Cache 的配置下，该函数为完全空操作。
     */
    EMPP_STATIC_INLINE void clean_addr(const uintptr_t addr,
                                       const size_t    len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
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

    /**
     * @brief 对指定内存区域执行 D-Cache 清除并失效 (clean + invalidate) 操作。
     *
     * @param addr 起始地址（任意对齐度）。
     * @param len  区域长度（字节数）。
     *
     * @details
     * - 先对全部覆盖区间执行 clean，再将对应 Cache Line 失效；
     * - 内部同样会做地址对齐与长度补齐；
     * - 实际调用 SCB_CleanInvalidateDCache_by_Addr()。
     *
     * 典型使用场景：
     * - 对读写混合或状态不清的区域进行“完全同步”，
     *   避免 Cache 和内存之间产生不一致。
     *
     * 在无 D-Cache 的配置下，该函数为完全空操作。
     */
    EMPP_STATIC_INLINE void
    clean_invalidate_addr(const uintptr_t addr, const size_t len) EMPP_NOEXCEPT
    {
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
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
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
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
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
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
