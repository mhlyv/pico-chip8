#include <iostream>
#include <cstdint>
#include <optional>

#include "chip8/memory.h"

namespace chip8
{
    void Memory::reset()
    {
        this->fill(0);
    }

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

    bool Memory::write(ptr_t ptr, std::uint8_t val)
    {
        bool res = false;

        if (this->size() > ptr)
        {
            this->operator[](ptr) = val;
            res = true;
        }

        return res;
    }

    std::optional<std::uint8_t> Memory::read(ptr_t ptr) const
    {
        std::optional<std::uint8_t> res = std::nullopt;

        if (this->size() > ptr)
        {
            res = this->operator[](ptr);
        }

        return res;
    }
};