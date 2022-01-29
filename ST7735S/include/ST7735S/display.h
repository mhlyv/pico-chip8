#pragma once

#include <cstdint>

#include "chip8/display.h"

namespace ST7735S
{
    // implementation of the display interface for the ST7735S display
    class Display : public chip8::Display
    {
    public:
        Display();

        void clear() const override;
        void draw(std::uint8_t x, std::uint8_t y, bool c) const override;
    };
};