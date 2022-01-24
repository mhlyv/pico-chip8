#pragma once

#include "chip8/memory.h"
#include "chip8/registers.h"
#include "chip8/stack.h"

namespace chip8
{
    class Machine
    {
    private:
        Memory memory;
        Registers registers;
        Stack stack;

    public:
        bool load(std::size_t n);
    };
};
