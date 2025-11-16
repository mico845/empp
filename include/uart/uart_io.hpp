#pragma once
#include "uart.hpp"

#include <cstdio>
#include <cstring>

namespace empp::platform::uart {

template <typename UartId>
class UartIO
{
public:
    using Base = Uart<UartId>;

    static void print(const char c) { Base::write(static_cast<uint8_t>(c)); }

    static void print(const char *s)
    {
        for (size_t i = 0; s[i] != '\0'; ++i) {
            Base::write(static_cast<uint8_t>(s[i]));
        }
    }

    static void print(const uint8_t *buf, const size_t len)
    {
        for (size_t i = 0; i < len; ++i) {
            Base::write(buf[i]);
        }
    }

    static void print(const int value)
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", value);
        print(buf);
    }

    template <typename... Args>
    static void println(const Args &...args)
    {
        print_any(args...);
        print("\r\n");
    }

private:
    // 多参数输出
    template <typename T>
    static void print_any(const T &t)
    {
        print(t);
    }

    template <typename T, typename... Args>
    static void print_any(const T &t, const Args &...rest)
    {
        print(t);
        print_any(rest...);
    }
};

} // namespace empp::platform::uart
