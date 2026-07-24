#include "alu.h"
#include <stdexcept>

int32_t ALU::execute(ALUOp op, int32_t a, int32_t b) {
    switch (op) {
        case ALUOp::ADD:  return a + b;
        case ALUOp::SUB:  return a - b;
        case ALUOp::AND:  return a & b;
        case ALUOp::OR:   return a | b;
        case ALUOp::XOR:  return a ^ b;

        case ALUOp::SLL:  return a << (b & 0x1F);
        case ALUOp::SRL:  return (uint32_t)a >> (b & 0x1F);
        case ALUOp::SRA:  return a >> (b & 0x1F);  // arithmetic — sign bit fills in

        case ALUOp::SLT:  return (a < b) ? 1 : 0;
        case ALUOp::SLTU: return ((uint32_t)a < (uint32_t)b) ? 1 : 0;

        case ALUOp::PASS: return a + b;  // address = base + offset

        default:
            throw std::runtime_error("Unknown ALU operation");
    }
}