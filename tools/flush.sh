#!/bin/sh
openocd -f interface/raspberrypi-swd.cfg \
    -f target/rp2040.cfg \
    -c "program build/pico-chip8.elf verify reset exit"