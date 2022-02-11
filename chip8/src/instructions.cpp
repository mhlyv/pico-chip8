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

        case 0x6:
        {
            load((inst & 0x0f00) >> 8, inst & 0x00ff);
            break;
        }

        case 0x7:
        {
            add((inst & 0x0f00) >> 8, inst & 0x00ff);
            break;
        }

        case 0x8:
        {
            switch (inst & 0x000f)
            {
            case 0:
            {
                loadr((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 1:
            {
                bin_or((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 2:
            {
                bin_and((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 3:
            {
                bin_xor((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 4:
            {
                add_carry((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 5:
            {
                sub((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 6:
            {
                shr((inst & 0x0f00) >> 8);
                break;
            }

            case 7:
            {
                subn((inst & 0x0f00) >> 8, (inst & 0x00f0) >> 4);
                break;
            }

            case 0xe:
            {
                shl((inst & 0x0f00) >> 8);
                break;
            }

            default:
            {
                err = Error::InvalidInstruction;
                break;
            }
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

    std::optional<Error> Machine::ret()
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

    std::optional<Error> Machine::jmp(ptr_t addr)
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

    std::optional<Error> Machine::call(ptr_t addr)
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

    std::optional<Error> Machine::skip_eq(std::uint8_t x, std::uint8_t kk)
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

    std::optional<Error> Machine::skip_neq(std::uint8_t x, std::uint8_t kk)
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

    std::optional<Error> Machine::skip_req(std::uint8_t x, std::uint8_t y)
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

    inline void Machine::load(std::uint8_t x, std::uint8_t kk)
    {
        registers.general[x] = kk;
    }

    inline void Machine::add(std::uint8_t x, std::uint8_t kk)
    {
        registers.general[x] += kk;
    }

    inline void Machine::loadr(std::uint8_t x, std::uint8_t y)
    {
        registers.general[x] = registers.general[y];
    }

    inline void Machine::bin_or(std::uint8_t x, std::uint8_t y)
    {
        registers.general[x] |= registers.general[y];
    }

    inline void Machine::bin_and(std::uint8_t x, std::uint8_t y)
    {
        registers.general[x] &= registers.general[y];
    }

    inline void Machine::bin_xor(std::uint8_t x, std::uint8_t y)
    {
        registers.general[x] ^= registers.general[y];
    }

    inline void Machine::add_carry(std::uint8_t x, std::uint8_t y)
    {
        const std::uint16_t res = (std::uint16_t)registers.general[x] + (std::uint16_t)registers.general[y];
        const std::uint8_t carry = (res & 0xff00) == 0 ? 0 : 1;

        registers.general[0xf] = carry;
        registers.general[x] = (res & 0x00ff);
    }

    inline void Machine::sub(std::uint8_t x, std::uint8_t y)
    {
        registers.general[0xf] = registers.general[x] > registers.general[y] ? 1 : 0;
        registers.general[x] -= registers.general[y];
    }

    inline void Machine::shr(std::uint8_t x)
    {
        registers.general[0xf] = registers.general[x] & 1;
        registers.general[x] >>= 1;
    }

    inline void Machine::subn(std::uint8_t x, std::uint8_t y)
    {
        registers.general[0xf] = registers.general[y] > registers.general[x] ? 1 : 0;
        registers.general[x] = registers.general[y] - registers.general[x];
    }

    inline void Machine::shl(std::uint8_t x)
    {
        registers.general[0xf] = registers.general[x] & 1 << 7;
        registers.general[x] <<= 1;
    }
};