#pragma once

#include <array>
#include <cstdint>

namespace chip8
{
    class Stack : private std::array<std::uint16_t, 16>
    {
    private:
        std::uint8_t stack_pointer = 0;

    public:
        bool push(std::uint16_t val);
        std::uint16_t pop();
        std::uint8_t size() const;
    };
};