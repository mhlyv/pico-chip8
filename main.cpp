#include <iostream>

#include "pico/stdlib.h"
#include "pico/platform.h"
#include "chip8/machine.h"

int main()
{
    stdio_init_all();

    // first byte seems to be always 255, I don't know why, so just ignore that
    std::cin.ignore();

    chip8::Machine machine;

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
            bool success = machine.load(n);
            if (success)
            {
                std::cout << "l ok\n";
            }
            else
            {
                std::cout << "l failed\n";
            }
            break;
        }

        default:
            // shouldn't happen, if it does, the desktop client fucked up
            panic("unexpected command: %c\n", cmd);
            break;
        }
    }
}
