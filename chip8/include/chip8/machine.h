#pragma once

#include <cstdint>
#include <optional>

#include "chip8/error.h"
#include "chip8/memory.h"
#include "chip8/registers.h"
#include "chip8/stack.h"
#include "chip8/display.h"

namespace chip8
{
    // an instruction in 16 bits
    typedef std::uint16_t inst_t;

    class Machine
    {
    private:
        Memory memory;
        Registers registers;
        Stack stack;
        Display &display;

        // try to read the next instruction from memory, and increment
        // the program counter accordingly
        inline std::optional<inst_t> fetch();

        // execute instruction
        std::optional<Error> exec(inst_t inst);

        // clear display
        inline void cls() const;

        // return from subroutine
        inline std::optional<Error> ret();

        // jump to address
        inline std::optional<Error> jmp(ptr_t addr);

        // call subroutine at address
        inline std::optional<Error> call(ptr_t addr);

        // skip next instruction if Vx == kk
        inline std::optional<Error> skip_eq(std::uint8_t x, std::uint8_t kk);

        // skip next instruction if Vx != kk
        inline std::optional<Error> skip_neq(std::uint8_t x, std::uint8_t kk);

        // skip next instruction if Vx != Vy
        inline std::optional<Error> skip_req(std::uint8_t x, std::uint8_t y);

        // load kk into Vx
        inline std::optional<Error> load(std::uint8_t x, std::uint8_t kk);

    public:
        Machine(Display &display);

        // reset everything, get ready for run
        void reset();

        // load n bytes into memory from stdin
        std::optional<Error> load(std::size_t n);

        // execute next instruction from memory
        std::optional<Error> exec_cycle();
    };
};
