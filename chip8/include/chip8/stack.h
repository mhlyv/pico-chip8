#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "chip8/error.h"

namespace chip8
{
    class Stack : private std::array<std::uint16_t, 16>
    {
    private:
        std::uint8_t stack_pointer = 0;

    public:
        // try to push a value on the stack, return StackOverflow if full
        std::optional<Error> push(std::uint16_t val);

        // try to pop a value from the stack
        std::optional<std::uint16_t> pop();

        // reset stack pointer, clear data
        void reset();
    };
};