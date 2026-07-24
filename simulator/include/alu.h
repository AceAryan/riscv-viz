#pragma once
#include <cstdint>

enum class ALUOp {
    ADD, SUB,
    AND, OR, XOR,
    SLL, SRL, SRA,  // shift left logical, shift right logical, shift right arithmetic
    SLT, SLTU,      // set less than (signed and unsigned)
    PASS             // just passes input through — used for load/store address calc
};

class ALU {
public:
    int32_t execute(ALUOp op, int32_t a, int32_t b);
};