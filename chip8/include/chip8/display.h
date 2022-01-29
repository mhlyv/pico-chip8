#pragma once

#include <cstdint>

namespace chip8
{
    /* 
    interface for chip8 display
    properties:
        - monochrome
        - 64 x 32 pixels
    */
    class Display
    {
    public:
        static const std::uint8_t size_x = 64;
        static const std::uint8_t size_y = 32;

        // clear display with black
        virtual void clear() const = 0;

        // draw pixel at (x y)
        virtual void draw(std::uint8_t x, std::uint8_t y, bool c) const = 0;

        virtual ~Display() {}
    };
};