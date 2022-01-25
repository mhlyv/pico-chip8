#include "chip8/stack.h"

namespace chip8
{
    void Stack::reset()
    {
        stack_pointer = 0;
        this->fill(0);
    }
};