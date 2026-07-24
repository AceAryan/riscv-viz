#include "instruction.h"

Instruction Instruction::decode(uint32_t raw) {
    Instruction instr;
    instr.raw    = raw;
    instr.opcode = raw & 0x7F;
    instr.rd     = (raw >> 7)  & 0x1F;
    instr.funct3 = (raw >> 12) & 0x7;
    instr.rs1    = (raw >> 15) & 0x1F;
    instr.rs2    = (raw >> 20) & 0x1F;
    instr.funct7 = (raw >> 25) & 0x7F;

    switch (instr.opcode) {
        case 0x33: instr.type = InstrType::R; break;  // add, sub, and, or
        case 0x13: instr.type = InstrType::I;         // addi
            instr.imm = (int32_t)raw >> 20;
            break;
        case 0x03: instr.type = InstrType::I;         // lw
            instr.imm = (int32_t)raw >> 20;
            break;
        case 0x23: instr.type = InstrType::S;         // sw
            instr.imm = ((raw >> 25) << 5) | ((raw >> 7) & 0x1F);
            instr.imm = (int32_t)(instr.imm << 20) >> 20;
            break;
        case 0x63: instr.type = InstrType::B;         // beq, bne
            instr.imm = ((raw>>31)<<12) | (((raw>>7)&1)<<11) |
                        (((raw>>25)&0x3F)<<5) | (((raw>>8)&0xF)<<1);
            instr.imm = (int32_t)(instr.imm << 19) >> 19;
            break;
        default:   instr.type = InstrType::UNKNOWN;
    }
    return instr;
}