#pragma once
#include <cstdint>

enum class InstrType { R, I, S, B, U, J, UNKNOWN };

struct Instruction {
    uint32_t raw;       // the original 32-bit binary instruction
    InstrType type;     // R / I / S / B / U / J
    uint32_t opcode;    // bits [6:0]  — tells you what kind of instruction
    int rd;             // bits [11:7] — destination register
    int rs1;            // bits [19:15] — source register 1
    int rs2;            // bits [24:20] — source register 2
    int32_t imm;        // immediate value (varies by type)
    uint32_t funct3;    // bits [14:12] — narrows down the operation
    uint32_t funct7;    // bits [31:25] — further narrows (R-type only)

    static Instruction decode(uint32_t raw);
};