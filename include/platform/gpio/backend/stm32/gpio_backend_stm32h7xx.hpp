// gpio_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.hpp"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/gpio/gpio_id.hpp"
    #include "empp/driver.hpp"

namespace empp::stm32h7xx::gpio {

template <platform::gpio::PinId ID>
struct Stm32H7Backend
{

    static constexpr uint32_t MASK       = (1u << ID.pin);
    static constexpr uint32_t MASK_RESET = (1u << (ID.pin + 16));

    EMPP_ALWAYS_INLINE static GPIO_TypeDef *regs() noexcept
    {
        if constexpr (ID.port == 0)
            return GPIOA;
        if constexpr (ID.port == 1)
            return GPIOB;
        if constexpr (ID.port == 2)
            return GPIOC;
        if constexpr (ID.port == 3)
            return GPIOD;
        if constexpr (ID.port == 4)
            return GPIOE;
        if constexpr (ID.port == 5)
            return GPIOF;
        if constexpr (ID.port == 6)
            return GPIOG;
        if constexpr (ID.port == 7)
            return GPIOH;
        return GPIOA;
    }

    EMPP_ALWAYS_INLINE static void set() noexcept { regs()->BSRR = MASK; }

    EMPP_ALWAYS_INLINE static void reset() noexcept
    {
        regs()->BSRR = MASK_RESET;
    }

    EMPP_ALWAYS_INLINE static void toggle() noexcept
    {
        if (auto *r = regs(); r->ODR & MASK)
            r->BSRR = MASK_RESET;
        else
            r->BSRR = MASK;
    }

    EMPP_ALWAYS_INLINE static bool read() noexcept
    {
        return (regs()->IDR & MASK) != 0;
    }
};

} // namespace empp::stm32h7xx::gpio

#endif