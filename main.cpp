#include <iostream>
#include <cstdlib>
#include <cstdint>

#include "pico/stdlib.h"
#include "pico/platform.h"
#include "hardware/regs/rosc.h"

#include "chip8/machine.h"
#include "ST7735S/display.h"

std::uint8_t get_rosc_rand()
{
    std::uint8_t byte = 0;
    volatile std::uint32_t *rnd_reg = (std::uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (std::uint8_t i = 0; i < 8; i++)
    {
        byte = (byte << 1) | (*rnd_reg & 1);
    }

    return byte;
}

// random number generator with FNV-1a hash algorithm
std::uint32_t hw_fnv_32()
{
    std::uint32_t hash = 0x811c9dc5;

    // hash 32 bytes of random data
    for (std::uint8_t i = 0; i < 32; i++)
    {
        hash ^= get_rosc_rand();
        hash *= 0x01000193;
    }

    return hash;
}

int main()
{
    std::srand(hw_fnv_32());
    stdio_init_all();

    // first byte seems to be always 255, I don't know why, so just ignore that
    std::cin.ignore();

    ST7735S::Display display;
    chip8::Machine machine(display);

    for (;;)
    {
        // single byte commands
        char cmd;
        std::cin >> cmd;

        switch (cmd)
        {
        case 'l':
        {
            std::size_t n;
            std::cin >> n;
            std::cin.ignore();
            auto err = machine.load(n);
            if (!err.has_value())
            {
                std::cout << "l ok\n";
            }
            else
            {
                std::cout << "l failed\n";
            }
            break;
        }

        case 'r':
        {
            machine.reset();
            std::optional<chip8::Error> err = std::nullopt;

            while (!err.has_value())
            {
                err = machine.exec_cycle();
            }

            std::cout << "r failed\n";
            break;
        }

        default:
            // shouldn't happen, if it does, the desktop client fucked up
            panic("unexpected command: %c\n", cmd);
            break;
        }
    }
}
