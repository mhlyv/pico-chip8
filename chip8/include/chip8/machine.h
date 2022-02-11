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
        std::optional<Error> ret();

        // jump to address
        std::optional<Error> jmp(ptr_t addr);

        // call subroutine at address
        std::optional<Error> call(ptr_t addr);

        // skip next instruction if Vx == kk
        std::optional<Error> skip_eq(std::uint8_t x, std::uint8_t kk);

        // skip next instruction if Vx != kk
        std::optional<Error> skip_neq(std::uint8_t x, std::uint8_t kk);

        // skip next instruction if Vx != Vy
        std::optional<Error> skip_req(std::uint8_t x, std::uint8_t y);

        // load kk into Vx
        inline void load(std::uint8_t x, std::uint8_t kk);

        // Vx = Vx + kk
        inline void add(std::uint8_t x, std::uint8_t kk);

        // load Vy into Vx
        inline void loadr(std::uint8_t x, std::uint8_t y);

        // Vx = Vx | Vy
        inline void bin_or(std::uint8_t x, std::uint8_t y);

        // Vx = Vx & Vy
        inline void bin_and(std::uint8_t x, std::uint8_t y);

        // Vx = Vx ^ Vy
        inline void bin_xor(std::uint8_t x, std::uint8_t y);

        // Vx = Vx + Vy, with carry bit in Vf
        inline void add_carry(std::uint8_t x, std::uint8_t y);

        // Vx = Vx - Vy, Vf = Vx > Vy ? 1 : 0
        inline void sub(std::uint8_t x, std::uint8_t y);

        // Vx = Vx >> 1
        inline void shr(std::uint8_t x);

        // Vx = Vy - Vx, Vf = Vy > Vx ? 1 : 0
        inline void subn(std::uint8_t x, std::uint8_t y);

        // Vx = Vx << 1
        inline void shl(std::uint8_t x);

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
