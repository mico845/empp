// dma_impl.hpp
#pragma once

#include "empp_config.h"
#include "empp/assert.hpp"
#include "dma_concept.hpp"
#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::dma {

template <DmaBackend Backend>
struct DmaImpl
{
    EMPP_STATIC_INLINE void configAddr(const uintptr_t peripheralAddr,
                                       const uintptr_t memoryAddr,
                                       const size_t    length) EMPP_NOEXCEPT
    {
#if defined(EMPP_DEBUG_TYPECHECK) && (EMPP_DEBUG_TYPECHECK == 1U)

        // 所有平台通用的基本约束
        EMPP_ASSERT((length > 0U),
                    "DMA transfer length must be greater than zero");
        EMPP_ASSERT((peripheralAddr != 0U),
                    "DMA peripheral address must be non-null");
        EMPP_ASSERT((memoryAddr != 0U), "DMA memory address must be non-null");

    #if defined(EMPP_CHIP_STM32H7)
        // H7 这层只做“位宽约束”，具体寄存器位数由 backend 决定
        // H7 经典 DMA1/DMA2 流控制器 NDTR 为 16 bit
        EMPP_ASSERT((length <= 0xFFFFu),
                    "STM32H7 DMA NDTR is 16-bit - length exceeds maximum");
    #endif

#endif // EMPP_DEBUG_TYPECHECK

        Backend::configAddr(peripheralAddr, memoryAddr, length);
    }
    EMPP_STATIC_INLINE void enable() EMPP_NOEXCEPT { Backend::enable(); }

    EMPP_STATIC_INLINE void disable() EMPP_NOEXCEPT { Backend::disable(); }

    EMPP_STATIC_INLINE void clear_all_flags() EMPP_NOEXCEPT
    {
        Backend::clear_all_flags();
    }

    [[nodiscard]] EMPP_STATIC_INLINE size_t get_length() EMPP_NOEXCEPT
    {
        return Backend::get_length();
    }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_tc() EMPP_NOEXCEPT
    {
        return Backend::is_tc();
    }

    EMPP_STATIC_INLINE void clear_tc() EMPP_NOEXCEPT { Backend::clear_tc(); }

    [[nodiscard]] EMPP_STATIC_INLINE bool is_ht() EMPP_NOEXCEPT
    {
        return Backend::is_ht();
    }

    EMPP_STATIC_INLINE void clear_ht() EMPP_NOEXCEPT { Backend::clear_ht(); }

    EMPP_STATIC_INLINE void enable_irq_tc() EMPP_NOEXCEPT
    {
        Backend::enable_irq_tc();
    }

    EMPP_STATIC_INLINE void disable_irq_tc() EMPP_NOEXCEPT
    {
        Backend::disable_irq_tc();
    }

    EMPP_STATIC_INLINE void enable_irq_ht() EMPP_NOEXCEPT
    {
        Backend::enable_irq_ht();
    }

    EMPP_STATIC_INLINE void disable_irq_ht() EMPP_NOEXCEPT
    {
        Backend::disable_irq_ht();
    }
};

} // namespace empp::platform::dma