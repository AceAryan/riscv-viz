#include <iostream>
#include "cpu.h"

int main() {
    CPU cpu;

    // addi x1, x0, 5
    // addi x2, x0, 10
    // add  x3, x1, x2
    uint32_t program[] = {
        0x00500093,  // addi x1, x0, 5
        0x00A00113,  // addi x2, x0, 10
        0x002081B3,  // add  x3, x1, x2
        0x00000000   // halt
    };

    cpu.loadProgram(program, 4);

    while (cpu.getState() == CPUState::RUNNING) {
        cpu.step();
        cpu.getRegs().print();
        std::cout << "---\n";
    }

    return 0;
}