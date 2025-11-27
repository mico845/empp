// dma_if_stm32h7xx.hpp
#pragma once

#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/dma/dma_impl.hpp"
    #include "platform/dma/backend/stm32/dma_backend_stm32h7xx.hpp"

namespace empp::stm32h7xx::dma {

template <DMAId Dmax, StreamId Strmx>
using Dma = platform::dma::DmaImpl<DmaBackend<Dmax, Strmx>>;

using Dma1S0 = Dma<DMAId::Dma1, StreamId::S0>;
using Dma1S1 = Dma<DMAId::Dma1, StreamId::S1>;
using Dma1S2 = Dma<DMAId::Dma1, StreamId::S2>;
using Dma1S3 = Dma<DMAId::Dma1, StreamId::S3>;
using Dma1S4 = Dma<DMAId::Dma1, StreamId::S4>;
using Dma1S5 = Dma<DMAId::Dma1, StreamId::S5>;
using Dma1S6 = Dma<DMAId::Dma1, StreamId::S6>;
using Dma1S7 = Dma<DMAId::Dma1, StreamId::S7>;

using Dma2S0 = Dma<DMAId::Dma2, StreamId::S0>;
using Dma2S1 = Dma<DMAId::Dma2, StreamId::S1>;
using Dma2S2 = Dma<DMAId::Dma2, StreamId::S2>;
using Dma2S3 = Dma<DMAId::Dma2, StreamId::S3>;
using Dma2S4 = Dma<DMAId::Dma2, StreamId::S4>;
using Dma2S5 = Dma<DMAId::Dma2, StreamId::S5>;
using Dma2S6 = Dma<DMAId::Dma2, StreamId::S6>;
using Dma2S7 = Dma<DMAId::Dma2, StreamId::S7>;

} // namespace empp::stm32h7xx::dma
#endif
