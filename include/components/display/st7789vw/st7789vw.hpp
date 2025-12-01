// st7789vw.hpp
#pragma once

#include "empp/type.hpp"
#include "empp/define.hpp"

#include "platform/gpio/gpio_concept.hpp"
#include "platform/delay/delay_concept.hpp"
#include "platform/spi/spi_concept.hpp"

namespace empp::componentBase::lcd::st7789vw {

template <platform::spi::SpiBackend Spi, platform::gpio::GpioBackend PinDC,
          platform::gpio::GpioBackend PinBL, platform::delay::DelayBackend Delay>
class ST7789VW
{
public:
    enum class Direction {
        Direction_H,
        Direction_V,
        Direction_H_Flip,
        Direction_V_Flip,
    };

    static constexpr uint16_t width  = 240;
    static constexpr uint16_t height = 280;

    // 卖家 Direction_V：X_Offset = 0, Y_Offset = 20
    static constexpr uint16_t x_offset = 0;
    static constexpr uint16_t y_offset = 20;

    EMPP_STATIC_INLINE void init() EMPP_NOEXCEPT
    {
        // 屏幕刚完成复位时（包括上电复位），需要等待至少5ms才能发送指令
        Delay::ms(10);

        // 软件复位
        write_command(0x01);
        Delay::ms(120);

        // 接下来很多都是电压设置指令，直接使用厂家给设定值
        write_command(0xB2);
        write_data8(0x0C);
        write_data8(0x0C);
        write_data8(0x00);
        write_data8(0x33);
        write_data8(0x33);

        write_command(0xB7);
        write_data8(0x35);

        write_command(0xBB);
        write_data8(0x19);

        write_command(0xC0);
        write_data8(0x2C);

        write_command(0xC2);
        write_data8(0x01);

        write_command(0xC3);
        write_data8(0x12);

        write_command(0xC4);
        write_data8(0x20);

        write_command(0xC6);
        write_data8(0x0F);

        write_command(0xD0);
        write_data8(0xA4);
        write_data8(0xA1);

        // 正伽玛
        write_command(0xE0);
        write_data8(0xD0);
        write_data8(0x04);
        write_data8(0x0D);
        write_data8(0x11);
        write_data8(0x13);
        write_data8(0x2B);
        write_data8(0x3F);
        write_data8(0x54);
        write_data8(0x4C);
        write_data8(0x18);
        write_data8(0x0D);
        write_data8(0x0B);
        write_data8(0x1F);
        write_data8(0x23);

        // 负伽玛
        write_command(0xE1);
        write_data8(0xD0);
        write_data8(0x04);
        write_data8(0x0C);
        write_data8(0x11);
        write_data8(0x13);
        write_data8(0x2C);
        write_data8(0x3F);
        write_data8(0x44);
        write_data8(0x51);
        write_data8(0x2F);
        write_data8(0x1F);
        write_data8(0x1F);
        write_data8(0x20);
        write_data8(0x23);

        // 打开反显，因为面板是常黑型，操作需要反过来
        write_command(0x21);

        // 退出休眠指令，LCD控制器在刚上电、复位时，会自动进入休眠模式
        // ，因此操作屏幕之前，需要退出休眠
        write_command(0x11);
        Delay::ms(120); // 需要等待120ms，让电源电压和时钟电路稳定下来

        // 显存访问控制：从上到下、从左到右，RGB 顺序
        write_command(0x36);
        write_data8(0x00);

        // 接口像素格式：16bit RGB565
        write_command(0x3A);
        write_data8(0x05);

        // 打开显示指令，LCD控制器在刚上电、复位时，会自动关闭显示
        write_command(0x29);

        // 最后再打开背光，避免初始化过程中闪屏
        PinBL::set();
    }

    // 全屏刷成一个 RGB565 颜色
    EMPP_STATIC_INLINE void fill_screen_rgb565(const uint16_t color) EMPP_NOEXCEPT
    {
        set_address_window(0, 0, width - 1, height - 1);

        Spi::set_dataWidth(16);
        PinDC::set();

        constexpr uint16_t BATCH = 50000;
        uint16_t           buf[BATCH];

        for (auto &v : buf)
            v = color;

        uint32_t remain = width * height;
        while (remain > 0) {
            uint16_t n = (remain > BATCH) ? BATCH : remain;
            Spi::write(buf, n);
            remain -= n;
        }
    }

private:
    // 写命令：DC=0
    EMPP_STATIC_INLINE void write_command(uint8_t cmd) EMPP_NOEXCEPT
    {
        Spi::set_dataWidth(8);
        PinDC::reset();
        Spi::template write<uint8_t>(cmd);
    }

    // 写数据：DC=1
    EMPP_STATIC_INLINE void write_data8(uint8_t data) EMPP_NOEXCEPT
    {
        Spi::set_dataWidth(8);
        PinDC::set();
        Spi::template write<uint8_t>(data);
    }

    EMPP_STATIC_INLINE void write_data16(uint16_t data) EMPP_NOEXCEPT
    {
        PinDC::set();
        Spi::template write<uint16_t>(data);
    }

    // 设置窗口：2Ah/2Bh，然后 2Ch
    EMPP_STATIC_INLINE void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1,
                                               uint16_t y1) EMPP_NOEXCEPT
    {
        const auto xs = static_cast<uint16_t>(x0 + x_offset);
        const auto xe = static_cast<uint16_t>(x1 + x_offset);
        const auto ys = static_cast<uint16_t>(y0 + y_offset);
        const auto ye = static_cast<uint16_t>(y1 + y_offset);

        // 列地址（X）
        write_command(0x2A);
        Spi::set_dataWidth(16);
        PinDC::set();
        Spi::template write<uint16_t>(xs);
        Spi::template write<uint16_t>(xe);

        // 行地址（Y）
        write_command(0x2B);
        Spi::set_dataWidth(16);
        PinDC::set();
        Spi::template write<uint16_t>(ys);
        Spi::template write<uint16_t>(ye);

        // 写 GRAM
        write_command(0x2C);
    }
};

} // namespace empp::componentBase::lcd::st7789vw
