#pragma once

#include <array>
#include <cstdint>

namespace chip8
{
    struct Registers
    {
        std::uint16_t I;
        ptr_t PC;
        std::array<std::uint8_t, 16> general;
    };
};