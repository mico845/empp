// st7789vw.hpp
#pragma once

#include "st7789vw_concept.hpp"

namespace empp::componentBase::lcd::st7789vw {

template <Spix Spi, Pinx PinDC, Pinx PinBL, Delayx Delay>
class ST7789VW
{
public:
    static constexpr uint16_t width  = 240;
    static constexpr uint16_t height = 280;

    // 卖家 Direction_V：X_Offset = 0, Y_Offset = 20
    static constexpr uint16_t x_offset = 0;
    static constexpr uint16_t y_offset = 20;

    EMPP_ALWAYS_INLINE static void init() noexcept
    {
        // 上电后先等一下
        Delay::ms(10);

        // 软件复位（RM 和多数 ST7789 例程都推荐先来一发）
        write_command(0x01);
        Delay::ms(120);

        // 显存访问控制：竖屏，从上到下、从左到右，RGB 顺序
        write_command(0x36);
        write_data(0x00); // == 卖家：LCD_SetDirection(Direction_V) 里的 0x00

        // 接口像素格式：16bit RGB565，卖家用 0x05
        write_command(0x3A);
        write_data(0x05);

        // ===== 以下照卖家的初始化表抄 =====
        write_command(0xB2);
        write_data(0x0C);
        write_data(0x0C);
        write_data(0x00);
        write_data(0x33);
        write_data(0x33);

        write_command(0xB7);
        write_data(0x35);

        write_command(0xBB);
        write_data(0x19);

        write_command(0xC0);
        write_data(0x2C);

        write_command(0xC2);
        write_data(0x01);

        write_command(0xC3);
        write_data(0x12);

        write_command(0xC4);
        write_data(0x20);

        write_command(0xC6);
        write_data(0x0F);

        write_command(0xD0);
        write_data(0xA4);
        write_data(0xA1);

        // 正伽玛
        write_command(0xE0);
        write_data(0xD0);
        write_data(0x04);
        write_data(0x0D);
        write_data(0x11);
        write_data(0x13);
        write_data(0x2B);
        write_data(0x3F);
        write_data(0x54);
        write_data(0x4C);
        write_data(0x18);
        write_data(0x0D);
        write_data(0x0B);
        write_data(0x1F);
        write_data(0x23);

        // 负伽玛
        write_command(0xE1);
        write_data(0xD0);
        write_data(0x04);
        write_data(0x0C);
        write_data(0x11);
        write_data(0x13);
        write_data(0x2C);
        write_data(0x3F);
        write_data(0x44);
        write_data(0x51);
        write_data(0x2F);
        write_data(0x1F);
        write_data(0x1F);
        write_data(0x20);
        write_data(0x23);

        // 打开反显（面板是常黑型）
        write_command(0x21);

        // 退出休眠
        write_command(0x11);
        Delay::ms(120);

        // 打开显示
        write_command(0x29);

        // 最后再打开背光，避免初始化过程中闪屏
        PinBL::set();
    }

    // 全屏刷成一个 RGB565 颜色
    EMPP_ALWAYS_INLINE static void
    fill_screen_rgb565(uint16_t color565) noexcept
    {
        set_address_window(0, 0, width - 1, height - 1);

        PinDC::set(); // 接下来是数据

        const uint8_t hi = static_cast<uint8_t>(color565 >> 8);
        const uint8_t lo = static_cast<uint8_t>(color565 & 0xFF);

        constexpr uint32_t total_pixels =
            static_cast<uint32_t>(width) * static_cast<uint32_t>(height);

        for (uint32_t i = 0; i < total_pixels; ++i) {
            Spi::write(hi);
            Spi::write(lo);
        }
    }

private:
    // 写命令：DC=0
    EMPP_ALWAYS_INLINE static void write_command(uint8_t cmd) noexcept
    {
        PinDC::reset();
        Spi::write(cmd);
    }

    // 写数据：DC=1
    EMPP_ALWAYS_INLINE static void write_data(uint8_t data) noexcept
    {
        PinDC::set();
        Spi::write(data);
    }

    // 设置窗口：2Ah/2Bh，然后 2Ch
    EMPP_ALWAYS_INLINE static void set_address_window(uint16_t x0, uint16_t y0,
                                                      uint16_t x1,
                                                      uint16_t y1) noexcept
    {
        const uint16_t xs = static_cast<uint16_t>(x0 + x_offset);
        const uint16_t xe = static_cast<uint16_t>(x1 + x_offset);
        const uint16_t ys = static_cast<uint16_t>(y0 + y_offset);
        const uint16_t ye = static_cast<uint16_t>(y1 + y_offset);

        // 列地址（X）
        write_command(0x2A);
        PinDC::set();
        Spi::write(static_cast<uint8_t>(xs >> 8));
        Spi::write(static_cast<uint8_t>(xs & 0xFF));
        Spi::write(static_cast<uint8_t>(xe >> 8));
        Spi::write(static_cast<uint8_t>(xe & 0xFF));

        // 行地址（Y）
        write_command(0x2B);
        PinDC::set();
        Spi::write(static_cast<uint8_t>(ys >> 8));
        Spi::write(static_cast<uint8_t>(ys & 0xFF));
        Spi::write(static_cast<uint8_t>(ye >> 8));
        Spi::write(static_cast<uint8_t>(ye & 0xFF));

        // 写 GRAM
        write_command(0x2C);
    }
};

} // namespace empp::componentBase::lcd::st7789vw
