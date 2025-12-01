// rgb_3gpio.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"
#include "platform/gpio/gpio_concept.hpp"

namespace empp::componentBase::rgb_3gpio {

enum class RGBPolarity : bool {
    CommonCathode, // 共阴，写 1 = 亮
    CommonAnode    // 共阳，写 0 = 亮
};

enum class RGBColor : uint8_t {
    Off = 0,
    Red,
    Green,
    Blue,
    Cyan,
    PinkishRed,
    Yellow,
    White,
};

struct RGBLogicalColor
{
    bool r;
    bool g;
    bool b;
};

template <platform::gpio::GpioBackend R, platform::gpio::GpioBackend G,
          platform::gpio::GpioBackend B, RGBPolarity Pol>
class RGB
{
public:
    EMPP_STATIC_INLINE void set(const bool r, const bool g, const bool b) EMPP_NOEXCEPT
    {
        set_channel<R>(r);
        set_channel<G>(g);
        set_channel<B>(b);
    }

    EMPP_STATIC_INLINE void off() EMPP_NOEXCEPT { set(false, false, false); }

    EMPP_STATIC_INLINE void red() EMPP_NOEXCEPT { set(RGBColor::Red); }

    EMPP_STATIC_INLINE void green() EMPP_NOEXCEPT { set(RGBColor::Green); }

    EMPP_STATIC_INLINE void blue() EMPP_NOEXCEPT { set(RGBColor::Blue); }

    EMPP_STATIC_INLINE void cyan() EMPP_NOEXCEPT { set(RGBColor::Cyan); }

    EMPP_STATIC_INLINE void pinkishRed() EMPP_NOEXCEPT { set(RGBColor::PinkishRed); }

    EMPP_STATIC_INLINE void yellow() EMPP_NOEXCEPT { set(RGBColor::Yellow); }

    EMPP_STATIC_INLINE void white() EMPP_NOEXCEPT { set(RGBColor::White); }

    EMPP_STATIC_INLINE void set(const RGBColor color) EMPP_NOEXCEPT
    {
        const auto [r, g, b] = color_from_id(color);
        set(r, g, b);
    }

private:
    EMPP_STATIC_INLINE constexpr RGBLogicalColor color_from_id(const RGBColor color) EMPP_NOEXCEPT
    {
        switch (color) {
            case RGBColor::Off:
                return {false, false, false};
            case RGBColor::Red:
                return {true, false, false};
            case RGBColor::Green:
                return {false, true, false};
            case RGBColor::Blue:
                return {false, false, true};
            case RGBColor::Cyan:
                return {false, true, true};
            case RGBColor::PinkishRed:
                return {true, false, true};
            case RGBColor::Yellow:
                return {true, true, false};
            case RGBColor::White:
                return {true, true, true};
        }

        return {false, false, false};
    }

    template <platform::gpio::GpioBackend P>
    EMPP_STATIC_INLINE void set_channel(const bool logical_on) EMPP_NOEXCEPT
    {
        constexpr bool active_high = (Pol == RGBPolarity::CommonCathode);
        if constexpr (active_high) {
            if (logical_on) {
                P::set();
            }
            else {
                P::reset();
            }
        }
        else {
            if (logical_on) {
                P::reset();
            }
            else {
                P::set();
            }
        }
    }
};
} // namespace empp::componentBase::rgb_3gpio
