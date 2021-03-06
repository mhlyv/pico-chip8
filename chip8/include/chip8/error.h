#pragma once

namespace chip8
{
    // runtime errors caused by execution of instructions in memory
    enum Error
    {
        PCOutOfBounds,
        AddressOutOfBounds,
        InvalidInstruction,
        StackOverflow,
        StackEmpty,
    };
};