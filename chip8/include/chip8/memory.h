#pragma once

#include <array>
#include <cstdint>
#include <optional>

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

        // write byte at location, returns success (bound checked)
        bool write(ptr_t ptr, std::uint8_t val);

        // read value (bound checked)
        std::optional<std::uint8_t> read(ptr_t ptr) const;
    };

    // entry point
    constexpr ptr_t program_start = 0x200;
};