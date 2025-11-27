// uart_io.hpp
#pragma once
#include "empp/type.hpp"
#include "uart_impl.hpp"

#include <cstdio>
#include <cstring>

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
        for (size_t i = 0; s[i] != '\0'; ++i) {
            Impl::write(static_cast<uint8_t>(s[i]));
        }
    }

    EMPP_ALWAYS_INLINE static void print(const uint8_t *buf,
                                         const size_t   len) noexcept
    {
        for (size_t i = 0; i < len; ++i) {
            Impl::write(buf[i]);
        }
    }

    EMPP_ALWAYS_INLINE static void print(const int value) noexcept
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", value);
        print(buf);
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
