project(ST7735S)

add_library(${PROJECT_NAME}
    src/display.cpp
)

target_link_libraries(${PROJECT_NAME}
    chip8
    pico_stdlib
    hardware_spi
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)