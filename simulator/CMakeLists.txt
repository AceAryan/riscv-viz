cmake_minimum_required(VERSION 3.15)
project(riscv-viz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiler warnings — good practice
add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

set(SOURCES
    src/cpu.cpp
    src/alu.cpp
    src/memory.cpp
    src/registers.cpp
    src/instruction.cpp
    src/parser.cpp
)

# main executable
add_executable(riscv-sim ${SOURCES} main.cpp)