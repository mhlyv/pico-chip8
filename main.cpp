#include <iostream>

#include "pico/stdlib.h"
#include "pico/platform.h"
#include "chip8/machine.h"
#include "ST7735S/display.h"

int main()
{
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
