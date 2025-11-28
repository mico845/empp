// dma_concept.hpp
#pragma once

#include "empp/type.hpp"

namespace empp::platform::dma {

template <typename T>
concept DmaBackend = requires(uint32_t addr1, uint32_t addr2, uint16_t length) {
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
    {
        T::clear_tc()
    } noexcept;
    {
        T::is_tc()
    } noexcept -> std::same_as<bool>;
    {
        T::clear_ht()
    } noexcept;
    {
        T::is_ht()
    } noexcept -> std::same_as<bool>;
    {
        T::enable_irq_tc()
    } noexcept;
    {
        T::disable_irq_tc()
    } noexcept;
    {
        T::enable_irq_ht()
    } noexcept;
    {
        T::disable_irq_ht()
    } noexcept;
    {
        T::get_length()
    } noexcept -> std::same_as<uint16_t>;
};
} // namespace empp::platform::dma
