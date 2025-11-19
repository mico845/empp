// rgb_3gpio.hpp
#pragma once
#include "rgb_3gpio_concept.hpp"
#include "empp/type.hpp"

namespace empp::componentBase::rgb_3gpio {

enum class RGBPolarity {
    CommonCathode, // 共阴，写 1 = 亮
    CommonAnode    // 共阳，写 0 = 亮
};

template <Pinx R, Pinx G, Pinx B, RGBPolarity Pol>
class RGB
{
public:
    EMPP_ALWAYS_INLINE static void off() noexcept
    {
        set_channel<R>(false);
        set_channel<G>(false);
        set_channel<B>(false);
    }

    EMPP_ALWAYS_INLINE static void red() noexcept
    {
        set_channel<R>(true);
        set_channel<G>(false);
        set_channel<B>(false);
    }

    EMPP_ALWAYS_INLINE static void green() noexcept
    {
        set_channel<R>(false);
        set_channel<G>(true);
        set_channel<B>(false);
    }

    EMPP_ALWAYS_INLINE static void blue() noexcept
    {
        set_channel<R>(false);
        set_channel<G>(false);
        set_channel<B>(true);
    }

    EMPP_ALWAYS_INLINE static void yellow() noexcept
    {
        set_channel<R>(true);
        set_channel<G>(true);
        set_channel<B>(false);
    }

    EMPP_ALWAYS_INLINE static void cyan() noexcept
    {
        set_channel<R>(false);
        set_channel<G>(true);
        set_channel<B>(true);
    }

    EMPP_ALWAYS_INLINE static void pinkishRed() noexcept
    {
        set_channel<R>(true);
        set_channel<G>(false);
        set_channel<B>(true);
    }

    EMPP_ALWAYS_INLINE static void white() noexcept
    {
        set_channel<R>(true);
        set_channel<G>(true);
        set_channel<B>(true);
    }

    EMPP_ALWAYS_INLINE static void set(const bool r_on, const bool g_on,
                                       const bool b_on) noexcept
    {
        set_channel<R>(r_on);
        set_channel<G>(g_on);
        set_channel<B>(b_on);
    }

private:
    template <Pinx P>
    EMPP_ALWAYS_INLINE static void set_channel(const bool level) noexcept
    {
        if constexpr (Pol == RGBPolarity::CommonCathode) {
            // 共阴
            if (level)
                P::set();
            else
                P::reset();
        }
        else {
            // 共阳
            if (level)
                P::reset();
            else
                P::set();
        }
    }
};
} // namespace empp::componentBase::rgb_3gpio
