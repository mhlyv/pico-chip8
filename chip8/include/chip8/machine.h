#pragma once

#include <cstdint>
#include <optional>

#include "chip8/memory.h"
#include "chip8/registers.h"
#include "chip8/stack.h"

namespace chip8
{
    // an instruction in 16 bits
    typedef std::uint16_t inst_t;

    // runtime errors caused by execution of instructions in memory
    enum Error
    {
        PCOutOfBounds,
    };

    class Machine
    {
    private:
        Memory memory;
        Registers registers;
        Stack stack;

        inline std::optional<std::uint8_t> fetch();

    public:
        // load n bytes into memory from stdin
        bool load(std::size_t n);

        // execute next instruction from memory
        std::optional<Error> exec_cycle();
    };
};
