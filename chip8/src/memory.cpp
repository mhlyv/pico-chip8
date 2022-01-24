#include <iostream>
#include <cstdint>

#include "chip8/memory.h"

namespace chip8
{
    bool Memory::load(std::size_t n)
    {
        bool success = false;

        if (n + program_start <= this->size())
        {
            for (size_t i = 0; i < n; i++)
            {
                this->operator[](i + program_start) = std::cin.get();
            }

            success = true;
        }

        return success;
    }
};