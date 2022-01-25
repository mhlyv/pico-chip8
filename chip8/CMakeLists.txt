project(chip8)

add_library(${PROJECT_NAME}
    src/machine.cpp
    src/memory.cpp
    src/registers.cpp
    src/stack.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)