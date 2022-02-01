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

    std::optional<Error> Memory::load(std::size_t n)
    {
        std::optional<Error> err = std::nullopt;

        if (n + program_start <= this->size())
        {
            for (size_t i = 0; i < n; i++)
            {
                this->operator[](i + program_start) = std::cin.get();
            }
        }
        else
        {
            err = Error::AddressOutOfBounds;
        }

        return err;
    }

    std::optional<Error> Memory::write(ptr_t ptr, std::uint8_t val)
    {
        std::optional<Error> err = std::nullopt;

        if (size() > ptr)
        {
            this->operator[](ptr) = val;
        }
        else
        {
            err = Error::AddressOutOfBounds;
        }

        return err;
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