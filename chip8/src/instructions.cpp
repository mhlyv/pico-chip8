#include <cstdint>
#include <optional>
#include <array>

#include "chip8/machine.h"

namespace chip8
{
    std::optional<Error> Machine::exec(inst_t inst)
    {
        std::optional<Error> err = std::nullopt;

        switch (inst >> 12)
        {
        case 0x0:
        {
            switch (inst)
            {
            case 0x00e0:
                cls();
                break;
            case 0x00ee:
                err = ret();
                break;
            default:
                err = Error::InvalidInstruction;
                break;
            }
            break;
        }

        default:
            err = Error::InvalidInstruction;
            break;
        }

        return err;
    }

    inline void Machine::cls() const
    {
        this->display.clear();
    }

    inline std::optional<Error> Machine::ret()
    {
        std::optional<Error> err = std::nullopt;

        auto addr = stack.pop();

        if (addr.has_value())
        {
            registers.PC = addr.value();
        }
        else
        {
            err = Error::StackEmpty;
        }

        return err;
    }

};