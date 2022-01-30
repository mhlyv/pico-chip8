#include <cstdint>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ST7735S/display.h"

namespace ST7735S
{
    constexpr std::uint8_t display_width = 160;
    constexpr std::uint8_t display_height = 80;
    auto *const spi = spi1;
    constexpr unsigned int spi_freq = 50000000U;
    constexpr unsigned int sck_pin = 14;
    constexpr unsigned int tx_pin = 15;
    constexpr unsigned int cs_pin = 13;
    constexpr unsigned int dc_pin = 12;
    constexpr unsigned int rst_pin = 11;
    constexpr std::uint8_t block_size = 2;
    constexpr std::uint8_t offset_x = (display_width - block_size * chip8::Display::size_x) / 2;
    constexpr std::uint8_t offset_y = (display_height - block_size * chip8::Display::size_y) / 2;

    // send a byte to the display
    inline void write_bus(std::uint8_t data)
    {
        gpio_put(cs_pin, 0);
        spi_write_blocking(spi, &data, 1);
        gpio_put(cs_pin, 1);
    }

    inline void write_8(std::uint8_t data)
    {
        gpio_put(dc_pin, 1);
        write_bus(data);
    }

    inline void write_16(std::uint16_t data)
    {
        gpio_put(dc_pin, 1);
        write_bus(data >> 8);
        write_bus(data);
    }

    // send command
    inline void write_command(std::uint8_t cmd)
    {
        gpio_put(dc_pin, 0);
        write_bus(cmd);
    }

    inline void init_gpio()
    {
        // initialize pins
        gpio_init(cs_pin);
        gpio_set_dir(cs_pin, GPIO_OUT);

        gpio_init(dc_pin);
        gpio_set_dir(dc_pin, GPIO_OUT);

        gpio_init(rst_pin);
        gpio_set_dir(rst_pin, GPIO_OUT);

        // initialize spi
        gpio_init(sck_pin);
        gpio_set_function(sck_pin, GPIO_FUNC_SPI);

        gpio_init(tx_pin);
        gpio_set_function(tx_pin, GPIO_FUNC_SPI);

        spi_init(spi, spi_freq);
        spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    }

    inline void set_window(std::uint16_t x1, std::uint16_t y1, std::uint16_t x2, std::uint16_t y2)
    {
        write_command(0x2a); // CASET
        write_16(x1 + 1);
        write_16(x2 + 1);

        write_command(0x2b); // RASET
        write_16(y1 + 26);
        write_16(y2 + 26);

        write_command(0x2c); // RAMWR
    }

    Display::Display()
    {
        init_gpio();
        gpio_put(cs_pin, 1);

        // hard reset
        gpio_put(rst_pin, 0);
        sleep_ms(200);
        gpio_put(rst_pin, 1);
        sleep_ms(20);

        // configure
        // documentation: http://myosuploads3.banggood.com/products/20190406/201904062255220.96LCD.rar

        write_command(0x11); // SLPOUT
        sleep_ms(120);

        write_command(0x21); // INVON

        write_command(0xb1); // FRMCTR1
        write_8(0x05);
        write_8(0x3A);
        write_8(0x3A);

        write_command(0xb2); // FRMCTR2
        write_8(0x05);
        write_8(0x3A);
        write_8(0x3A);

        write_command(0xb3); // FRMCTR3
        write_8(0x05);
        write_8(0x3A);
        write_8(0x3A);
        write_8(0x05);
        write_8(0x3A);
        write_8(0x3A);

        write_command(0xb4); // INVCTR
        write_8(0x03);

        write_command(0xc0); // PWCTR1
        write_8(0x62);
        write_8(0x02);
        write_8(0x04);

        write_command(0xc1); // PWCTR2
        write_8(0xC0);

        write_command(0xc2); // PWCTR3
        write_8(0x0D);
        write_8(0x00);

        write_command(0xc3); // PWCTR4
        write_8(0x8D);
        write_8(0x6A);

        write_command(0xc4); // PWCTR5
        write_8(0x8D);
        write_8(0xEE);

        write_command(0xc5); // VMCTR1
        write_8(0x0E);

        write_command(0xe0); // GMCTRP1
        write_8(0x10);
        write_8(0x0E);
        write_8(0x02);
        write_8(0x03);
        write_8(0x0E);
        write_8(0x07);
        write_8(0x02);
        write_8(0x07);
        write_8(0x0A);
        write_8(0x12);
        write_8(0x27);
        write_8(0x37);
        write_8(0x00);
        write_8(0x0D);
        write_8(0x0E);
        write_8(0x10);

        write_command(0xe1); // GMCTRN1
        write_8(0x10);
        write_8(0x0E);
        write_8(0x03);
        write_8(0x03);
        write_8(0x0F);
        write_8(0x06);
        write_8(0x02);
        write_8(0x08);
        write_8(0x0A);
        write_8(0x13);
        write_8(0x26);
        write_8(0x36);
        write_8(0x00);
        write_8(0x0D);
        write_8(0x0E);
        write_8(0x10);

        write_command(0x3A); // COLMOD
        write_8(0x05);

        write_command(0x36); // MADCTL
        write_8(0x78);

        write_command(0x29); // DISPON
        clear();
    }

    void Display::clear() const
    {
        set_window(0, 0, display_width - 1, display_height - 1);
        for (std::uint16_t i = 0; i < display_height * display_width; i++)
        {
            write_16(0x0000);
        }
    }

    void Display::draw(std::uint8_t chip8_x, std::uint8_t chip8_y, bool c) const
    {
        const std::uint8_t x1 = chip8_x * block_size + offset_x;
        const std::uint8_t y1 = chip8_y * block_size + offset_y;
        const std::uint8_t x2 = x1 + block_size - 1;
        const std::uint8_t y2 = y1 + block_size - 1;
        const std::uint16_t color = c ? 0xffff : 0x0000;

        set_window(x1, y1, x2, y2);

        for (std::uint8_t i = 0; i < block_size * block_size; i++)
        {
            write_16(color);
        }
    }
};