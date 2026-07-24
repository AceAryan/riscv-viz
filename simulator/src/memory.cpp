#include "memory.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Memory::Memory() {
    reset();
}

void Memory::reset() {
    mem.fill(0);
}

// ── loads ──────────────────────────────────────────

uint32_t Memory::loadWord(uint32_t addr) {
    if (addr % 4 != 0)
        throw std::runtime_error("Unaligned word access at 0x" + std::to_string(addr));
    if (addr + 3 >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));

    // little-endian: byte 0 is least significant
    return  (uint32_t)mem[addr]     |
            (uint32_t)mem[addr + 1] << 8  |
            (uint32_t)mem[addr + 2] << 16 |
            (uint32_t)mem[addr + 3] << 24;
}

uint16_t Memory::loadHalf(uint32_t addr) {
    if (addr % 2 != 0)
        throw std::runtime_error("Unaligned halfword access at 0x" + std::to_string(addr));
    if (addr + 1 >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));

    return  (uint16_t)mem[addr] |
            (uint16_t)mem[addr + 1] << 8;
}

uint8_t Memory::loadByte(uint32_t addr) {
    if (addr >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));
    return mem[addr];
}

// ── stores ─────────────────────────────────────────

void Memory::storeWord(uint32_t addr, uint32_t value) {
    if (addr % 4 != 0)
        throw std::runtime_error("Unaligned word access at 0x" + std::to_string(addr));
    if (addr + 3 >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));

    mem[addr]     =  value        & 0xFF;
    mem[addr + 1] = (value >> 8)  & 0xFF;
    mem[addr + 2] = (value >> 16) & 0xFF;
    mem[addr + 3] = (value >> 24) & 0xFF;
}

void Memory::storeHalf(uint32_t addr, uint16_t value) {
    if (addr % 2 != 0)
        throw std::runtime_error("Unaligned halfword access at 0x" + std::to_string(addr));
    if (addr + 1 >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));

    mem[addr]     =  value       & 0xFF;
    mem[addr + 1] = (value >> 8) & 0xFF;
}

void Memory::storeByte(uint32_t addr, uint8_t value) {
    if (addr >= MEM_SIZE)
        throw std::runtime_error("Memory out of bounds at 0x" + std::to_string(addr));
    mem[addr] = value;
}

// ── utility ────────────────────────────────────────

void Memory::loadProgram(const uint32_t* instructions, int count) {
    for (int i = 0; i < count; i++)
        storeWord(i * 4, instructions[i]);
}

void Memory::print(uint32_t from, uint32_t to) {
    for (uint32_t addr = from; addr < to; addr += 4) {
        std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << addr
                  << " : " << std::setw(8) << loadWord(addr) << "\n";
    }
    std::cout << std::dec;
}