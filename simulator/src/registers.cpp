#include "registers.h"
#include <iostream>
#include <iomanip>

RegisterFile::RegisterFile() {
    reset();
}

void RegisterFile::reset() {
    regs.fill(0);  // all registers start at 0
}

int32_t RegisterFile::read(int index) {
    if (index < 0 || index > 31) {
        std::cerr << "Invalid register index: " << index << "\n";
        return 0;
    }
    return regs[index];  // x0 naturally returns 0 since it's always 0
}

void RegisterFile::write(int index, int32_t value) {
    if (index < 0 || index > 31) {
        std::cerr << "Invalid register index: " << index << "\n";
        return;
    }
    if (index == 0) return;  // x0 is hardwired to 0, writes are ignored
    regs[index] = value;
}

void RegisterFile::print() {
    for (int i = 0; i < 32; i++) {
        std::cout << "x" << std::setw(2) << std::setfill('0') << i
                  << " = " << std::setw(10) << regs[i];
        if ((i + 1) % 4 == 0) std::cout << "\n";
        else std::cout << "  |  ";
    }
}