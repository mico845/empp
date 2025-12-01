// gpio_backend_stm32h7xx.hpp
#pragma once
#include "empp_config.h"

#if defined(EMPP_CHIP_STM32H7)

    #include "platform/gpio/gpio_id.hpp"
    #include "empp/driver.hpp"

namespace empp::stm32h7xx::gpio {

/**
 * @brief 使能 I/O 速度优化（高速低压补偿单元）。
 *
 * @details
 * 设置 SYSCFG_CCCSR.HSLV 位，启用 GPIO 高速低压补偿单元，用于在
 * VDD 低于 2.7V 时提高 I/O 速度裕量。
 *
 * @warning
 * 仅当产品供电电压低于 2.7V 时才必须使用此位。
 * 当 VDD 高于 2.7V 时设置该位可能会损坏器件，请确保硬件电压条件满足手册要求。
 */
__used EMPP_STATIC_INLINE void enable_speed_optimization() EMPP_NOEXCEPT
{
    SYSCFG->CCCSR |= SYSCFG_CCCSR_HSLV;
}

/**
 * @brief 禁用 I/O 速度优化（高速低压补偿单元）。
 *
 * @details
 * 清除 SYSCFG_CCCSR.HSLV 位，关闭 GPIO 高速低压补偿单元。
 */
__used EMPP_STATIC_INLINE void disable_speed_optimization() EMPP_NOEXCEPT
{
    SYSCFG->CCCSR &= ~SYSCFG_CCCSR_HSLV;
}

/**
 * @brief STM32H7 单个 GPIO 引脚 Backend。
 *
 * @tparam ID 编译期 GPIO 引脚标识，包含端口号和引脚号：
 *             - ID.port : 端口索引，GPIOA=0, GPIOB=1, ..., GPIOH=7
 *             - ID.pin  : 引脚索引，范围 0..15
 *
 */
template <platform::gpio::PinId ID>
struct GpioBackend
{
    static_assert(ID.port <= 7u,
                  "Invalid GPIO port: only A(0)~H(7) are supported on STM32H7");
    static_assert(ID.pin <= 15u,
                  "Invalid GPIO pin: only pin 0~15 are valid for STM32H7");

    /// @brief 单个引脚对应的 bit 掩码 (用于 SET 操作)。
    static constexpr uint32_t MASK = (uint32_t{1} << ID.pin);

    /// @brief 单个引脚对应的 bit 掩码 (用于 RESET 操作, 写 BSRR 的高 16 位)。
    static constexpr uint32_t MASK_RESET = (uint32_t{1} << (ID.pin + 16U));

    /**
     * @brief 获取当前 GPIO 引脚所属端口的寄存器基址。
     *
     * @return 对应 GPIO_TypeDef* 寄存器指针。
     *
     * @warning
     * - ID.port 必须在 [0,7] 范围内，对应 GPIOA~GPIOH。
     */
    EMPP_STATIC_INLINE constexpr GPIO_TypeDef *regs() EMPP_NOEXCEPT
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

        __builtin_unreachable();
    }

    /**
     * @brief 将引脚输出置为高电平。
     *
     * @details
     * 向 BSRR 写 MASK，通过“bit set”单 bit 原子设置对应 ODR 位为 1。
     */
    EMPP_STATIC_INLINE void set() EMPP_NOEXCEPT { regs()->BSRR = MASK; }

    /**
     * @brief 将引脚输出置为低电平。
     *
     * @details
     * 向 BSRR 写 MASK_RESET，通过“bit reset”单 bit 原子清除对应 ODR 位为 0。
     */
    EMPP_STATIC_INLINE void reset() EMPP_NOEXCEPT { regs()->BSRR = MASK_RESET; }

    /**
     * @brief 反转引脚输出电平（toggle）。
     *
     * @details
     * 为避免对 ODR 寄存器进行 read-modify-write 操作导致其它 bit 受到影响，
     * 本函数的实现为：
     * - 先读取 ODR 是否为 1；
     * - 再通过 BSRR 仅对该 bit 执行 SET 或 RESET。
     *
     * 这样可以保证对该 bit 的原子操作，不影响同一端口中其它引脚的状态。
     */
    EMPP_STATIC_INLINE void toggle() EMPP_NOEXCEPT
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
        if (auto * const r = regs(); (r->ODR & MASK) != 0U) {
            r->BSRR = MASK_RESET;
        }
        else {
            r->BSRR = MASK;
        }
    #endif
    }

    /**
     * @brief 读取引脚当前输入电平。
     *
     * @retval true  引脚电平为高（IDR 对应位为 1）。
     * @retval false 引脚电平为低（IDR 对应位为 0）。
     *
     * @note
     * - 读取的是 IDR（输入数据寄存器），与 ODR 可能不同步，
     *   具体取决于引脚配置为输入/输出模式以及外部电路。
     */
    EMPP_STATIC_INLINE bool read() EMPP_NOEXCEPT
    {
        return (regs()->IDR & MASK) != 0U;
    }
};

} // namespace empp::stm32h7xx::gpio

#endif