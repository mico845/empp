// rgb_3gpio.hpp
#pragma once
#include "rgb_3gpio_concept.hpp"
#include "empp/type.hpp"

namespace empp::componentBase::rgb_3gpio {

enum class RGBPolarity {
    CommonCathode, // 共阴，写 1 = 亮
    CommonAnode    // 共阳，写 0 = 亮
};

enum class RGBColorId : uint8_t {
    Off = 0,
    Red,
    Green,
    Blue,
    Cyan,
    pinkishRed,
    Yellow,
    White,
};

struct RGBLogicalColor
{
    bool r;
    bool g;
    bool b;
};

template <Pinx R, Pinx G, Pinx B, RGBPolarity Pol>
class RGB
{
public:
    EMPP_ALWAYS_INLINE static void set(const bool r, const bool g,
                                       const bool b) noexcept
    {
        set_channel<R>(r);
        set_channel<G>(g);
        set_channel<B>(b);
    }

    EMPP_ALWAYS_INLINE static void off() noexcept { set(false, false, false); }

    EMPP_ALWAYS_INLINE static void red() noexcept { set(RGBColorId::Red); }

    EMPP_ALWAYS_INLINE static void green() noexcept { set(RGBColorId::Green); }

    EMPP_ALWAYS_INLINE static void blue() noexcept { set(RGBColorId::Blue); }

    EMPP_ALWAYS_INLINE static void cyan() noexcept { set(RGBColorId::Cyan); }

    EMPP_ALWAYS_INLINE static void pinkishRed() noexcept
    {
        set(RGBColorId::pinkishRed);
    }

    EMPP_ALWAYS_INLINE static void yellow() noexcept
    {
        set(RGBColorId::Yellow);
    }

    EMPP_ALWAYS_INLINE static void white() noexcept { set(RGBColorId::White); }

private:
    static constexpr RGBLogicalColor kColorTable[] = {
        /* Off     */ {false, false, false},
        /* Red     */ {true, false, false},
        /* Green   */ {false, true, false},
        /* Blue    */ {false, false, true},
        /* Cyan    */ {false, true, true},
        /* pinkishRed */ {true, false, true},
        /* Yellow  */ {true, true, false},
        /* White   */ {true, true, true},
    };

    template <Pinx P>
    EMPP_ALWAYS_INLINE static void set_channel(const bool logical_on) noexcept
    {
        constexpr bool active_high = (Pol == RGBPolarity::CommonCathode);
        (active_high == logical_on) ? P::set() : P::reset();
    }

    EMPP_ALWAYS_INLINE static void set(RGBColorId color_id) noexcept
    {
        const auto idx = static_cast<uint8_t>(color_id);

        const auto &[r, g, b] = kColorTable[idx];
        set(r, g, b);
    }
};
} // namespace empp::componentBase::rgb_3gpio
