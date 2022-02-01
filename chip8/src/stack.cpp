#include <cstdint>
#include <optional>

#include "chip8/stack.h"
#include "chip8/error.h"

namespace chip8
{
    std::optional<Error> Stack::push(std::uint16_t val)
    {
        std::optional<Error> err = std::nullopt;

        if (stack_pointer >= size())
        {
            err = Error::StackOverflow;
        }
        else
        {
            operator[](stack_pointer) = val;
            stack_pointer += 1;
        }

        return err;
    }

    std::optional<std::uint16_t> Stack::pop()
    {
        std::optional<std::uint16_t> ret = std::nullopt;

        if (stack_pointer != 0)
        {
            stack_pointer -= 1;
            ret = operator[](stack_pointer);
        }

        return ret;
    }

    void Stack::reset()
    {
        stack_pointer = 0;
        this->fill(0);
    }
};