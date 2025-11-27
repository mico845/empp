// dma_concept.hpp
#pragma once

#include "empp/type.hpp"

namespace empp::platform::dma {

template <typename T>
concept DmaBackend = requires(uint32_t addr1, uint32_t addr2, uint32_t length) {
    {
        T::configAddr(addr1, addr2, length)
    } noexcept;
    {
        T::enable()
    } noexcept;
    {
        T::disable()
    } noexcept;
    {
        T::clear_all_flags()
    } noexcept;
};

} // namespace empp::platform::dma
