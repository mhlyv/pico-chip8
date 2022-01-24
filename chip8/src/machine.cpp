#include "chip8/machine.h"

namespace chip8
{
    bool Machine::load(std::size_t n)
    {
        return memory.load(n);
    }
};