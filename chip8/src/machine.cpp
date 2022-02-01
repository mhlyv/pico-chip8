#include <cstdint>
#include <optional>

#include "chip8/machine.h"
#include "chip8/memory.h"
#include "chip8/display.h"

namespace chip8
{
    Machine::Machine(Display &display) : display(display) {}

    void Machine::reset()
    {
        this->registers.reset();
        this->stack.reset();
    }

    bool Machine::load(std::size_t n)
    {
        memory.reset();
        return memory.load(n);
    }

    inline std::optional<inst_t> Machine::fetch()
    {
        std::optional<inst_t> ret = std::nullopt;
        const std::optional<inst_t> msb = this->memory.read(this->registers.PC);
        const std::optional<inst_t> lsb = this->memory.read(this->registers.PC + 1);

        this->registers.PC += 2;

        if (msb.has_value() && lsb.has_value())
        {
            ret = msb.value() << 8 | lsb.value();
        }

        return ret;
    }

    std::optional<Error> Machine::exec_cycle()
    {
        std::optional<Error> err = std::nullopt;
        const auto instruction = fetch();

        if (instruction.has_value())
        {
            err = exec(instruction.value());
        }
        else
        {
            // the program counter went out of the memory
            err = Error::PCOutOfBounds;
        }

        return err;
    }
};