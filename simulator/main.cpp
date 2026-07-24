#include <iostream>
#include <fstream>
#include "cpu.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: riscv-sim <file.s> [max_cycles]\n";
        return 1;
    }

    int maxCycles = argc > 2 ? std::stoi(argv[2]) : 99999;

    CPU cpu;
    Parser parser;

    auto instructions = parser.parseFile(argv[1]);
    cpu.loadProgram(instructions.data(), instructions.size());

    for (int i = 0; i < maxCycles && cpu.getState() == CPUState::RUNNING; i++)
        cpu.step();

    // force decimal for all output
    std::cout << std::dec;

    auto& regs = cpu.getRegs();
    auto& mem  = cpu.getMem();

    std::cout << "{\n";
    std::cout << "  \"pc\": "     << cpu.getPC()        << ",\n";
    std::cout << "  \"cycle\": "  << cpu.getCycleCount() << ",\n";
    std::cout << "  \"halted\": " << (cpu.getState() == CPUState::HALTED ? "true" : "false") << ",\n";

    std::cout << "  \"registers\": [";
    for (int i = 0; i < 32; i++) {
        std::cout << regs.read(i);
        if (i < 31) std::cout << ", ";
    }
    std::cout << "],\n";

    std::cout << "  \"memory\": [";
    for (int i = 0; i < 256; i++) {
        std::cout << (int)mem.loadByte(i);
        if (i < 255) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "}\n";

    return 0;
}