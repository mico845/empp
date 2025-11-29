// dma_concept.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::dma {

template <typename T>
concept DmaBackend =
    requires(uintptr_t addr1, uintptr_t addr2, typename T::length_type length) {
        {
            T::configAddr(addr1, addr2, length)
        }
        EMPP_NOEXCEPT;
        {
            T::enable()
        }
        EMPP_NOEXCEPT;
        {
            T::disable()
        }
        EMPP_NOEXCEPT;
        {
            T::clear_all_flags()
        }
        EMPP_NOEXCEPT;
        {
            T::clear_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::is_tc()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;
        {
            T::clear_ht()
        }
        EMPP_NOEXCEPT;
        {
            T::is_ht()
        }
        EMPP_NOEXCEPT->std::same_as<bool>;
        {
            T::enable_irq_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_tc()
        }
        EMPP_NOEXCEPT;
        {
            T::enable_irq_ht()
        }
        EMPP_NOEXCEPT;
        {
            T::disable_irq_ht()
        }
        EMPP_NOEXCEPT;
        {
            T::get_length()
        }
        EMPP_NOEXCEPT->std::same_as<typename T::length_type>;
    };
} // namespace empp::platform::dma
