#include <cstdint>
#include <optional>

#include "chip8/machine.h"
#include "chip8/memory.h"

namespace chip8
{
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

    // read byte from memory at the program counter, then increment the program counter
    inline std::optional<inst_t> Machine::fetch()
    {
        const auto ret = this->memory.read(this->registers.PC);
        this->registers.PC++;
        return ret;
    }

    std::optional<Error> Machine::exec_cycle()
    {
        const std::optional<inst_t> msb = fetch();
        const std::optional<inst_t> lsb = fetch();
        std::optional<Error> err = std::nullopt;

        // check if we actually got the bytes
        if (msb.has_value() && lsb.has_value())
        {
            // safe to unwrap instuction
            inst_t instruction = msb.value() << 8 | lsb.value();

            // TODO handle instruction
        }
        else
        {
            // the program counter isn't valid
            err = Error::PCOutOfBounds;
        }

        return err;
    }
};