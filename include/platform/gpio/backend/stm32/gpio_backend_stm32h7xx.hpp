// gpio_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/gpio/gpio_id.hpp"
    #include "empp/driver.hpp"

namespace empp::stm32h7xx::gpio {

/*
 * 当产品电压较低时启用 I/O 速度优化
 * 仅产品供电电压低于 2.7 V 时才必须使用此位。当 VDD 高于 2.7 V
 * 时设置此位可能会造成破坏。
 * 详情可以参考 STM32H7
 * 开启IO补偿单元和关闭补偿单元，GPIO配置不同速度等级的最高速度
 */
EMPP_ALWAYS_INLINE static void enable_speed_optimization() noexcept
{
    SYSCFG->CCCSR |= SYSCFG_CCCSR_HSLV;
}

EMPP_ALWAYS_INLINE static void disable_speed_optimization() noexcept
{
    SYSCFG->CCCSR &= ~SYSCFG_CCCSR_HSLV;
}

template <platform::gpio::PinId ID>
struct GpioBackend
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
        /*
         * 电平反转不选择使用 regs()->ODR ^= MASK;
         * 是因为这一句话其实是三步操作：
         * 1. 读 ODR     ODR = GPIO->ODR;
         * 2. 异或 MASK   ODR ^= MASK;
         * 3. 写回 ODR    GPIO->ODR = ODR;
         * 可见其存在非线程安全行为，如中断修改 ODR 值，会导致 新的ODR 被旧的
         */
    #if 0
        regs()->ODR ^= MASK;
    #else
        if (auto *r = regs(); r->ODR & MASK)
            r->BSRR = MASK_RESET;
        else
            r->BSRR = MASK;
    #endif
    }

    EMPP_ALWAYS_INLINE static bool read() noexcept
    {
        return (regs()->IDR & MASK) != 0;
    }
};

} // namespace empp::stm32h7xx::gpio

#endif