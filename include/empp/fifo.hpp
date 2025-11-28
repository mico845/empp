// fifo.hpp
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lwrb/lwrb.h"

#ifdef __cplusplus
}

    #include "empp/define.hpp"
    #include <type_traits>

namespace empp {

template <typename T, lwrb_sz_t N>
class fifo
{
    static_assert(N > 0, "empp::fifo size must be > 0");
    static_assert(std::is_trivially_copyable_v<T>,
                  "empp::fifo<T> requires trivially copyable T");

public:
    using value_type = T;
    using size_type  = lwrb_sz_t;

    static constexpr size_type kCapacity = N;

    EMPP_ALWAYS_INLINE fifo() noexcept { init_(); }

    fifo(const fifo &)            = delete;
    fifo &operator=(const fifo &) = delete;

    /// 清空 FIFO（不改变容量）
    EMPP_ALWAYS_INLINE void reset() noexcept { lwrb_reset(&rb_); }

    /// 当前已存元素个数
    [[nodiscard]] EMPP_ALWAYS_INLINE size_type size() const noexcept
    {
        const lwrb_sz_t full_bytes = lwrb_get_full(&rb_);
        return full_bytes / sizeof(T);
    }

    /// 逻辑容量（元素数）
    [[nodiscard]] static constexpr EMPP_ALWAYS_INLINE size_type
    capacity() noexcept
    {
        return kCapacity;
    }

    /// 剩余可写元素个数
    [[nodiscard]] EMPP_ALWAYS_INLINE size_type available() const noexcept
    {
        // 依赖于“所有写操作均为 sizeof(T) 的整数倍”的约束
        return kCapacity - size();
    }

    /// 是否为空
    [[nodiscard]] EMPP_ALWAYS_INLINE bool empty() const noexcept
    {
        return size() == 0;
    }

    /// 是否“看起来满了”（已用元素数 >= 容量）
    [[nodiscard]] EMPP_ALWAYS_INLINE bool full() const noexcept
    {
        return size() >= kCapacity;
    }

    /// 写入一个元素，成功返回 true
    EMPP_ALWAYS_INLINE bool push(const T &v) noexcept
    {
        return write(&v, 1) == 1;
    }

    /// 读出一个元素，成功返回 true
    EMPP_ALWAYS_INLINE bool pop(T &out) noexcept { return read(&out, 1) == 1; }

    /// 写入最多 count 个元素，返回实际写入的元素个数
    EMPP_ALWAYS_INLINE size_type write(const T        *data,
                                       const size_type count) noexcept
    {
        const lwrb_sz_t bytes   = static_cast<lwrb_sz_t>(count) * sizeof(T);
        const lwrb_sz_t written = lwrb_write(&rb_, data, bytes);
        return written / sizeof(T);
    }

    EMPP_ALWAYS_INLINE size_type push(const T        *data,
                                    const size_type count) noexcept
    {
        const lwrb_sz_t bytes   = static_cast<lwrb_sz_t>(count) * sizeof(T);
        const lwrb_sz_t written = lwrb_write(&rb_, data, bytes);
        return written / sizeof(T);
    }

    /// 读出最多 count 个元素，返回实际读出的元素个数
    EMPP_ALWAYS_INLINE size_type read(T *data, const size_type count) noexcept
    {
        const lwrb_sz_t bytes = static_cast<lwrb_sz_t>(count) * sizeof(T);
        const lwrb_sz_t read  = lwrb_read(&rb_, data, bytes);
        return read / sizeof(T);
    }

    EMPP_ALWAYS_INLINE size_type pop(T *data, const size_type count) noexcept
    {
        const lwrb_sz_t bytes = static_cast<lwrb_sz_t>(count) * sizeof(T);
        const lwrb_sz_t read  = lwrb_read(&rb_, data, bytes);
        return read / sizeof(T);
    }

    /// 从头部开始 peek count 个元素，不移动读指针，可指定跳过 skip 个元素
    EMPP_ALWAYS_INLINE size_type peek(T *data, const size_type count,
                                      const size_type skip = 0) const noexcept
    {
        const lwrb_sz_t skip_bytes = skip * sizeof(T);
        const lwrb_sz_t peek_bytes = static_cast<lwrb_sz_t>(count) * sizeof(T);
        const lwrb_sz_t taken_bytes =
            lwrb_peek(&rb_, skip_bytes, data, peek_bytes);
        return taken_bytes / sizeof(T);
    }

    /* DMA / 零拷贝友好接口（按元素计数） */

    /// 获取线性可读块起始指针（TX: memory -> peripheral）
    EMPP_ALWAYS_INLINE T *linear_read_ptr() noexcept
    {
        return static_cast<T *>(lwrb_get_linear_block_read_address(&rb_));
    }

    /// 获取线性可读块长度（单位：元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE size_type
    linear_read_length() const noexcept
    {
        const lwrb_sz_t len_bytes = lwrb_get_linear_block_read_length(&rb_);
        return len_bytes / sizeof(T);
    }

    /// 标记消费 count 个元素（配合 DMA TX 完成中断）
    EMPP_ALWAYS_INLINE void skip(const size_type count) noexcept
    {
        (void)lwrb_skip(&rb_, count * sizeof(T));
    }

    /// 获取线性可写块起始指针（RX: peripheral -> memory）
    EMPP_ALWAYS_INLINE T *linear_write_ptr() noexcept
    {
        return static_cast<T *>(lwrb_get_linear_block_write_address(&rb_));
    }

    /// 获取线性可写块长度（单位：元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE size_type
    linear_write_length() const noexcept
    {
        const lwrb_sz_t len_bytes = lwrb_get_linear_block_write_length(&rb_);
        return len_bytes / sizeof(T);
    }

    /// 写指针前进 count 个元素（配合 DMA RX 完成中断）
    EMPP_ALWAYS_INLINE void advance(const size_type count) noexcept
    {
        (void)lwrb_advance(&rb_, static_cast<lwrb_sz_t>(count) * sizeof(T));
    }

    /* 底层句柄（必要时可直接使用 LwRB 原始 API，如 lwrb_write_ex 等） */

    EMPP_ALWAYS_INLINE lwrb_t *native_handle() noexcept { return &rb_; }

    [[nodiscard]] EMPP_ALWAYS_INLINE const lwrb_t *
    native_handle() const noexcept
    {
        return &rb_;
    }

    /* ---------- 暴露内部存储（用于 DCache / 特殊 DMA 操作） ---------- */

    /// 底层原始存储区（按 T* 视角，长度为 N+1 个元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE value_type *buffer() noexcept
    {
        return reinterpret_cast<value_type *>(storage_);
    }

    [[nodiscard]] EMPP_ALWAYS_INLINE const value_type *buffer() const noexcept
    {
        return reinterpret_cast<const value_type *>(storage_);
    }

    /// 底层原始存储区（按字节视角）
    [[nodiscard]] EMPP_ALWAYS_INLINE void *buffer_bytes() noexcept
    {
        return static_cast<void *>(storage_);
    }

    [[nodiscard]] EMPP_ALWAYS_INLINE const void *buffer_bytes() const noexcept
    {
        return static_cast<const void *>(storage_);
    }

    /// 底层存储区总大小（单位：字节），供 DCache 清/失效使用
    [[nodiscard]] constexpr EMPP_ALWAYS_INLINE size_type
    buffer_size_bytes() noexcept
    {
        return kStorageSizeBytes;
    }

private:
    // LwRB 的 size 字段是“字节数”，最大有效数据为 size-1 字节。
    // 为了对外容量为 N 个 T，这里分配 (N + 1) * sizeof(T) 字节。
    static constexpr lwrb_sz_t kStorageSizeBytes = (N + 1u) * sizeof(T);

    alignas(T) uint8_t storage_[kStorageSizeBytes] = {};
    lwrb_t rb_{};

    EMPP_ALWAYS_INLINE void init_() noexcept
    {
        (void)lwrb_init(&rb_, storage_, kStorageSizeBytes);
    }
};

} // namespace empp

#endif