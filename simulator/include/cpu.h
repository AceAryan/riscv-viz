#pragma once
#include <cstdint>
#include "registers.h"
#include "memory.h"
#include "alu.h"
#include "instruction.h"

enum class CPUState { RUNNING, HALTED };

class CPU {
private:
    RegisterFile regs;
    Memory mem;
    ALU alu;
    uint32_t PC;
    CPUState state;
    int cycleCount;

    void execute(const Instruction& instr);
    ALUOp resolveALUOp(const Instruction& instr);

public:
    CPU();
    void reset();
    void step();
    void run(int maxCycles = 10000);
    void loadProgram(const uint32_t* instructions, int count);

    // state accessors — frontend will call these via API
    uint32_t getPC()         const { return PC; }
    int getCycleCount()      const { return cycleCount; }
    CPUState getState()      const { return state; }
    RegisterFile& getRegs()        { return regs; }
    Memory& getMem()               { return mem; }
};  