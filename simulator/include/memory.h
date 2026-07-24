#pragma once
#include <cstdint>
#include <array>

#define MEM_SIZE 65536  // 64KB

class Memory {
private:
    std::array<uint8_t, MEM_SIZE> mem;

public:
    Memory();
    void reset();

    uint32_t loadWord(uint32_t addr);     // lw  — 4 bytes
    uint16_t loadHalf(uint32_t addr);     // lh  — 2 bytes
    uint8_t  loadByte(uint32_t addr);     // lb  — 1 byte

    void storeWord(uint32_t addr, uint32_t value);   // sw
    void storeHalf(uint32_t addr, uint16_t value);   // sh
    void storeByte(uint32_t addr, uint8_t value);    // sb

    void loadProgram(const uint32_t* instructions, int count);
    void print(uint32_t from, uint32_t to);  // debug — print memory range
};