cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)
project(pico-chip8 VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

pico_sdk_init()

add_subdirectory(chip8)
add_subdirectory(ST7735S)

add_executable(pico-chip8 main.cpp)
target_link_libraries(pico-chip8
    pico_stdlib
    hardware_regs
    chip8
    ST7735S
)

pico_enable_stdio_usb(pico-chip8 0)
pico_enable_stdio_uart(pico-chip8 1)

pico_add_extra_outputs(pico-chip8)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
