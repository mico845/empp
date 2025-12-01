// spi_concept.hpp
#pragma once
#include "empp/type.hpp"
#include "empp/define.hpp"

namespace empp::platform::spi {

template <typename T>
concept SpiBackend =
    std::is_empty_v<T> /* 保证 0 开销 */
    && requires(uint8_t v8, uint16_t v16, uint8_t *p8, uint16_t *p16, size_t count, uint8_t N) {
           {
               T::write(p8, count)
           }
           EMPP_NOEXCEPT;

           {
               T::write(p16, count)
           }
           EMPP_NOEXCEPT;

           {
               T::template write<uint8_t>(v8)
           }
           EMPP_NOEXCEPT;
           {
               T::template write<uint16_t>(v16)
           }
           EMPP_NOEXCEPT;

           {
               T::set_dataWidth(N)
           }
           EMPP_NOEXCEPT;
       };

} // namespace empp::platform::spi
