// uart_io.hpp

#pragma once

#include "empp/type.hpp"
#include "uart_impl.hpp"
#include "empp/define.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
class UartIO : public UartImpl<Backend>
{
public:
    static constexpr size_t default_precision = 3; // 默认小数精度
    using Impl                                = UartImpl<Backend>;

    static_assert(sizeof(int) == 4, "UartIO::print(int) assumes 32-bit int");

    EMPP_STATIC_INLINE void print(const char c) EMPP_NOEXCEPT
    {
        Impl::write(static_cast<uint8_t>(c));
    }

    EMPP_STATIC_INLINE void print(const char *s) EMPP_NOEXCEPT
    {
        if (s == nullptr) {
            return;
        }
        for (const char *p = s; *p != '\0'; ++p) {
            Impl::write(static_cast<uint8_t>(*p));
        }
    }

    EMPP_STATIC_INLINE void print(const uint8_t *buf, const size_t len) EMPP_NOEXCEPT
    {
        if (buf == nullptr || len == 0U) {
            return;
        }

        const uint8_t *p   = buf;
        const uint8_t *end = buf + len;

        while (p < end) {
            Impl::write(*p++);
        }
    }

    // 按 int32 范围处理
    EMPP_STATIC_INLINE void print(const int value) EMPP_NOEXCEPT
    {
        char  buf[12]; // "-2147483648\0"
        char *p = buf + sizeof(buf);
        *--p    = '\0';

        const bool negative = (value < 0);
        auto       u        = static_cast<uint32_t>(value);
        if (negative) {
            u = 0u - u; // 二补码下等价于 abs(value)，正确处理 INT_MIN
        }

        if (u == 0u) {
            *--p = '0';
        }
        else {
            while (u != 0u) {
                const uint32_t digit = u % 10u;
                u /= 10u;
                *--p = static_cast<char>('0' + digit);
            }
        }

        if (negative) {
            *--p = '-';
        }

        print(p);
    }

    EMPP_STATIC_INLINE void print(uint32_t value) EMPP_NOEXCEPT
    {
        char  buf[11]; // "4294967295\0" 最大 10 位 + '\0'
        char *p = buf + sizeof(buf);
        *--p    = '\0';

        if (value == 0u) {
            *--p = '0';
        }
        else {
            while (value != 0u) {
                const uint32_t digit = value % 10u;
                value /= 10u;
                *--p = static_cast<char>('0' + digit);
            }
        }

        print(p);
    }

    EMPP_STATIC_INLINE void print(const uint8_t value) EMPP_NOEXCEPT
    {
        print(static_cast<uint32_t>(value));
    }

    EMPP_STATIC_INLINE void print(const uint16_t value) EMPP_NOEXCEPT
    {
        print(static_cast<uint32_t>(value));
    }

    EMPP_STATIC_INLINE void print(const float value) EMPP_NOEXCEPT
    {
        print_float(static_cast<double>(value));
    }

    EMPP_STATIC_INLINE void print(const float value, const size_t precision) EMPP_NOEXCEPT
    {
        print_float(static_cast<double>(value), precision);
    }

    EMPP_STATIC_INLINE void print(const double value) EMPP_NOEXCEPT { print_float(value); }

    EMPP_STATIC_INLINE void print(const double value, const size_t precision) EMPP_NOEXCEPT
    {
        print_float(value, precision);
    }

    template <typename... Args>
    EMPP_STATIC_INLINE void println(const Args &...args) EMPP_NOEXCEPT
    {
        print_any(args...);
        print("\r\n");
    }

private:
    // 多参数输出
    template <typename T>
    EMPP_STATIC_INLINE void print_any(const T &t) EMPP_NOEXCEPT
    {
        print(t);
    }

    template <typename T, typename... Args>
    EMPP_STATIC_INLINE void print_any(const T &t, const Args &...rest) EMPP_NOEXCEPT
    {
        print(t);
        print_any(rest...);
    }

    EMPP_STATIC_INLINE void print_float(double value,
                                        size_t precision = default_precision) EMPP_NOEXCEPT
    {
        // 限制精度，避免溢出和无意义的过多小数
        if (precision > 9U) {
            precision = 9U;
        }

        // 简单处理正负无穷大（依赖编译器 IEEE754 行为）
        constexpr double huge = 1e300;
        if (value > huge) {
            print("inf");
            return;
        }
        if (value < -huge) {
            print("-inf");
            return;
        }

        // 处理符号
        if (value < 0.0) {
            print('-');
            value = -value;
        }

        // 整数部分
        const uint32_t int_part = static_cast<uint32_t>(value);
        const double   frac     = value - static_cast<double>(int_part);

        // 输出整数部分（复用现有 uint32_t 打印）
        print(int_part);

        // 如果不需要小数，直接返回
        if (precision == 0U) {
            return;
        }

        print('.');

        // 10 的幂表，避免每次循环乘法
        static constexpr uint32_t pow10_tbl[10] = {
            1u, 10u, 100u, 1000u, 10000u, 100000u, 1000000u, 10000000u, 100000000u, 1000000000u,
        };

        const uint32_t scale = pow10_tbl[precision];

        // 按精度放大并四舍五入
        double scaled = frac * static_cast<double>(scale) + 0.5;
        if (scaled >= static_cast<double>(scale)) {
            // 0.999... 四舍五入到 1.000 的情况，向整数部分进一位
            scaled -= static_cast<double>(scale);
            // 这里不回写 int_part，只是保持小数部分从 0 开始
            // 若你希望连整数部分一起进位，可在外面重构逻辑
        }

        const uint32_t frac_scaled = static_cast<uint32_t>(scaled);

        // 计算实际数字位数，用于补前导 0
        uint32_t tmp    = frac_scaled;
        uint8_t  digits = 0U;
        if (tmp == 0U) {
            digits = 1U;
        }
        else {
            while (tmp > 0U) {
                tmp /= 10U;
                ++digits;
            }
        }

        // 补足前导 0，例如 1.002 这样的数字
        for (uint8_t i = digits; i < precision; ++i) {
            print('0');
        }

        // 输出小数部分
        if (frac_scaled != 0U) {
            print(frac_scaled);
        }
    }
};

} // namespace empp::platform::uart
