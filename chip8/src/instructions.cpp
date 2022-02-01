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

        case 0x1:
        {
            err = jmp(inst & 0x0fff);
            break;
        }

        case 0x2:
        {
            err = call(inst & 0x0fff);
            break;
        }

        case 0x3:
        {
            err = skip_eq((inst & 0x0f00) >> 8, inst & 0x00ff);
            break;
        }

        case 0x4:
        {
            err = skip_neq((inst & 0x0f00) >> 8, inst & 0x00ff);
            break;
        }

        case 0x5:
        {
            if ((inst & 0x000f) == 0)
            {
                err = skip_req((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
            }
            else
            {
                err = Error::InvalidInstruction;
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

    inline std::optional<Error> Machine::jmp(ptr_t addr)
    {
        std::optional<Error> err = std::nullopt;

        // I don't know if I should check if the address is even

        if (addr >= memory.size())
        {
            err = Error::AddressOutOfBounds;
        }
        else
        {
            registers.PC = addr;
        }

        return err;
    }

    inline std::optional<Error> Machine::call(ptr_t addr)
    {
        std::optional<Error> err = std::nullopt;

        if (addr >= memory.size())
        {
            err = Error::AddressOutOfBounds;
        }
        else
        {
            err = stack.push(registers.PC);

            if (!err.has_value())
            {
                registers.PC = addr;
            }
        }

        return err;
    }

    inline std::optional<Error> Machine::skip_eq(std::uint8_t x, std::uint8_t kk)
    {
        std::optional<Error> err = std::nullopt;

        if (registers.general[x] == kk)
        {
            if (registers.PC + 2 >= memory.size())
            {
                err = Error::AddressOutOfBounds;
            }
            else
            {
                registers.PC += 2;
            }
        }

        return err;
    }

    inline std::optional<Error> Machine::skip_neq(std::uint8_t x, std::uint8_t kk)
    {
        std::optional<Error> err = std::nullopt;

        if (registers.general[x] != kk)
        {
            if (registers.PC + 2 >= memory.size())
            {
                err = Error::AddressOutOfBounds;
            }
            else
            {
                registers.PC += 2;
            }
        }

        return err;
    }

    inline std::optional<Error> Machine::skip_req(std::uint8_t x, std::uint8_t y)
    {
        std::optional<Error> err = std::nullopt;

        if (registers.general[x] == registers.general[y])
        {
            if (registers.PC + 2 >= memory.size())
            {
                err = Error::AddressOutOfBounds;
            }
            else
            {
                registers.PC += 2;
            }
        }

        return err;
    }
};