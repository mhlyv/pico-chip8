#pragma once

#include <cstdint>
#include <optional>

#include "chip8/memory.h"
#include "chip8/registers.h"
#include "chip8/stack.h"
#include "chip8/display.h"

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
        Display &display;

        // try to read the next instruction from memory, and increment
        // the program counter accordingly
        inline std::optional<inst_t> fetch();

    public:
        Machine(Display &display);

        // reset everything, get ready for run
        void reset();

        // load n bytes into memory from stdin
        bool load(std::size_t n);

        // execute next instruction from memory
        std::optional<Error> exec_cycle();
    };
};
