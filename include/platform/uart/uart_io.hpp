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
    using Impl = UartImpl<Backend>;

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

    EMPP_STATIC_INLINE void print(const uint8_t *buf,
                                  const size_t   len) EMPP_NOEXCEPT
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
    EMPP_STATIC_INLINE void print_any(const T &t,
                                      const Args &...rest) EMPP_NOEXCEPT
    {
        print(t);
        print_any(rest...);
    }
};

} // namespace empp::platform::uart
