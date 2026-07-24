#include <iostream>
#include "cpu.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    CPU cpu;
    Parser parser;

    std::string filename = argc > 1 ? argv[1] : "../tests/fibonacci.s";

    auto instructions = parser.parseFile(filename);
    cpu.loadProgram(instructions.data(), instructions.size());

    while (cpu.getState() == CPUState::RUNNING)
        cpu.step();

    cpu.getRegs().print();
    std::cout << "Cycles: " << cpu.getCycleCount() << "\n";

    return 0;
}