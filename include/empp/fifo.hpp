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
    #include <limits>

namespace empp {

template <typename T, lwrb_sz_t N>
class fifo
{
    static_assert(N > 0, "empp::fifo size must be > 0");
    static_assert(std::is_trivially_copyable_v<T>,
                  "empp::fifo<T> requires trivially copyable T");

public:
    using size_type = lwrb_sz_t;

    static constexpr size_type kCapacity = N;
    static constexpr size_type kElemSize = static_cast<size_type>(sizeof(T));

    // LwRB 的 size 字段是“字节数”，最大有效数据为 size-1 字节。
    // 为了对外容量为 N 个 T，这里分配 (N + 1) * sizeof(T) 字节。
    static constexpr size_type kStorageSizeBytes = (N + 1U) * kElemSize;

    static_assert(kStorageSizeBytes > 0, "empp::fifo storage size must be > 0");
    static_assert(kStorageSizeBytes <= std::numeric_limits<size_type>::max(),
                  "empp::fifo storage size does not fit in size_type");

    EMPP_ALWAYS_INLINE fifo() EMPP_NOEXCEPT { init_(); }

    /// 清空 FIFO（不改变容量）
    EMPP_ALWAYS_INLINE void reset() EMPP_NOEXCEPT { lwrb_reset(&rb_); }

    /// 获取缓冲区中可用于写入操作的大小
    [[nodiscard]] EMPP_ALWAYS_INLINE size_t
    available_write() const EMPP_NOEXCEPT
    {
        const size_type write_bytes = lwrb_get_free(&rb_);
        return static_cast<size_t>(write_bytes / kElemSize);
    }

    /// 获取缓冲区中当前可读的字节数
    [[nodiscard]] EMPP_ALWAYS_INLINE size_t available_read() const EMPP_NOEXCEPT
    {
        const size_type read_bytes = lwrb_get_full(&rb_);
        return static_cast<size_t>(read_bytes / kElemSize);
    }

    /// 写入一个元素，成功返回 true
    EMPP_ALWAYS_INLINE bool write(const T &in) EMPP_NOEXCEPT
    {
        return (lwrb_write(&rb_, &in, kElemSize) / kElemSize) == 1U;
    }

    /// 将数据写入缓冲区
    EMPP_ALWAYS_INLINE size_t write(const T     *data,
                                   const size_t count) EMPP_NOEXCEPT
    {
        const size_type bytes   = static_cast<size_type>(count) * kElemSize;
        const size_type written = lwrb_write(&rb_, data, bytes);
        return static_cast<size_t>(written / kElemSize);
    }

    /// 读出一个元素，成功返回 true
    EMPP_ALWAYS_INLINE bool read(T &out) EMPP_NOEXCEPT
    {
        return (lwrb_read(&rb_, &out, kElemSize) / kElemSize) == 1U;
    }

    /// 从缓冲区读取数据
    EMPP_ALWAYS_INLINE size_t read(T *data, const size_t count) EMPP_NOEXCEPT
    {
        const size_type bytes = static_cast<size_type>(count) * kElemSize;
        const size_type read  = lwrb_read(&rb_, data, bytes);
        return static_cast<size_t>(read / kElemSize);
    }

    /// 从缓冲区读取而不改变读取指针（仅预览）
    EMPP_ALWAYS_INLINE size_t peek(T *data, const size_t count,
                                   const size_t skip = 0) const EMPP_NOEXCEPT
    {
        const size_type skip_bytes = skip * kElemSize;
        const size_type peek_bytes = static_cast<size_type>(count) * kElemSize;
        const size_type taken_bytes =
            lwrb_peek(&rb_, skip_bytes, data, peek_bytes);
        return static_cast<size_t>(taken_bytes / kElemSize);
    }

    /* DMA / 零拷贝友好接口（按元素计数） */

    /// 获取线性可读块起始指针（TX: memory -> peripheral）
    EMPP_ALWAYS_INLINE T *linear_read_ptr() EMPP_NOEXCEPT
    {
        return static_cast<T *>(lwrb_get_linear_block_read_address(&rb_));
    }

    /// 获取线性可读块长度（单位：元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE size_t
    linear_read_length() const EMPP_NOEXCEPT
    {
        const size_type len_bytes = lwrb_get_linear_block_read_length(&rb_);
        return static_cast<size_t>(len_bytes / kElemSize);
    }

    /// 标记消费 count 个元素（配合 DMA TX 完成中断）
    EMPP_ALWAYS_INLINE void skip(const size_t count) EMPP_NOEXCEPT
    {
        (void)lwrb_skip(&rb_, static_cast<size_type>(count) * kElemSize);
    }

    /// 获取线性可写块起始指针（RX: peripheral -> memory）
    EMPP_ALWAYS_INLINE T *linear_write_ptr() EMPP_NOEXCEPT
    {
        return static_cast<T *>(lwrb_get_linear_block_write_address(&rb_));
    }

    /// 获取线性可写块长度（单位：元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE size_t
    linear_write_length() const EMPP_NOEXCEPT
    {
        const size_type len_bytes = lwrb_get_linear_block_write_length(&rb_);
        return static_cast<size_t>(len_bytes / kElemSize);
    }

    /// 写指针前进 count 个元素（配合 DMA RX 完成中断）
    EMPP_ALWAYS_INLINE void advance(const size_t count) EMPP_NOEXCEPT
    {
        (void)lwrb_advance(&rb_, static_cast<size_type>(count) * kElemSize);
    }

    /* 底层句柄（必要时可直接使用 LwRB 原始 API，如 lwrb_write_ex 等） */

    EMPP_ALWAYS_INLINE lwrb_t *native_handle() EMPP_NOEXCEPT { return &rb_; }

    [[nodiscard]] EMPP_ALWAYS_INLINE const lwrb_t *
    native_handle() const EMPP_NOEXCEPT
    {
        return &rb_;
    }

    /* ---------- 暴露内部存储（用于 DCache / 特殊 DMA 操作） ---------- */

    /// 底层原始存储区（按 T* 视角，长度为 N+1 个元素）
    [[nodiscard]] EMPP_ALWAYS_INLINE T *buffer() EMPP_NOEXCEPT
    {
        return reinterpret_cast<T *>(storage_);
    }

    [[nodiscard]] EMPP_ALWAYS_INLINE const T *buffer() const EMPP_NOEXCEPT
    {
        return reinterpret_cast<const T *>(storage_);
    }

    /// 底层原始存储区（按字节视角）
    [[nodiscard]] EMPP_ALWAYS_INLINE void *buffer_bytes() EMPP_NOEXCEPT
    {
        return static_cast<void *>(storage_);
    }

    [[nodiscard]] EMPP_ALWAYS_INLINE const void *
    buffer_bytes() const EMPP_NOEXCEPT
    {
        return static_cast<const void *>(storage_);
    }

    /// 底层存储区总大小（单位：字节），供 DCache 清/失效使用
    [[nodiscard]] static constexpr EMPP_ALWAYS_INLINE size_type
    buffer_size_bytes() EMPP_NOEXCEPT
    {
        return kStorageSizeBytes;
    }

private:
    alignas(T) uint8_t storage_[kStorageSizeBytes] = {};
    lwrb_t rb_{};

    EMPP_ALWAYS_INLINE void init_() EMPP_NOEXCEPT
    {
        lwrb_init(&rb_, storage_, kStorageSizeBytes);
    }
};

} // namespace empp

#endif