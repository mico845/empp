// uart_io.hpp
#pragma once
#include "empp/type.hpp"
#include "uart_impl.hpp"

namespace empp::platform::uart {

template <UartBackend Backend>
class UartIO : public UartImpl<Backend>
{
public:
    using Impl = UartImpl<Backend>;

    EMPP_ALWAYS_INLINE static void print(const char c) noexcept
    {
        Impl::write(static_cast<uint8_t>(c));
    }

    EMPP_ALWAYS_INLINE static void print(const char *s) noexcept
    {
        if (s == nullptr)
            return;
        for (const char *p = s; *p != '\0'; ++p) {
            Impl::write(static_cast<uint8_t>(*p));
        }
    }

    EMPP_ALWAYS_INLINE static void print(const uint8_t *buf,
                                         const size_t   len) noexcept
    {
        if (buf == nullptr)
            return;

        const uint8_t *p   = buf;
        const uint8_t *end = buf + len;

        while (p < end) {
            Impl::write(*p++);
        }
    }

    EMPP_ALWAYS_INLINE static void print(const int value) noexcept
    {
        char     buf[12]; // 足够容纳 int32: "-2147483648\0"
        char    *p   = buf + sizeof(buf);
        *--p        = '\0';

        const bool     negative = (value < 0); // 将有符号数安全地转换为无符号再取绝对值，避免 UB
        auto u = static_cast<uint32_t>(value);
        if (negative) {
            u = 0u - u; // 二补码下等价于 abs(value)
        }

        // 处理 0
        if (u == 0u) {
            *--p = '0';
        } else {
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

    template <typename... Args>
    EMPP_ALWAYS_INLINE static void println(const Args &...args) noexcept
    {
        print_any(args...);
        print("\r\n");
    }

private:
    // 多参数输出
    template <typename T>
    EMPP_ALWAYS_INLINE static void print_any(const T &t) noexcept
    {
        print(t);
    }

    template <typename T, typename... Args>
    EMPP_ALWAYS_INLINE static void print_any(const T &t,
                                             const Args &...rest) noexcept
    {
        print(t);
        print_any(rest...);
    }
};

} // namespace empp::platform::uart
