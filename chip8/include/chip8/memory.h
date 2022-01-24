#pragma once

#include <array>
#include <cstdint>

namespace chip8
{
    // memory is addressable with 12 bits
    typedef std::uint16_t ptr_t;

    // memory is an array of bytes
    typedef std::array<std::uint8_t, 0xfff> Memory;

    // entry point
    constexpr ptr_t program_start = 0x200;
};