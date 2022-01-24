#pragma once

#include <array>
#include <cstdint>

namespace chip8
{
    // memory is addressable with 12 bits
    typedef std::uint16_t ptr_t;

    // memory is an array of bytes
    class Memory : private std::array<std::uint8_t, 0xfff>
    {
    public:
        // loads n bytes from the input stream into memory starting at program_start
        // returns false on failure
        bool load(std::size_t n);
    };

    // entry point
    constexpr ptr_t program_start = 0x200;
};