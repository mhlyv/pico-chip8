#include "chip8/registers.h"

namespace chip8
{
    void Registers::reset()
    {
        I = 0;
        PC = program_start;
        general.fill(0);
    }
};
