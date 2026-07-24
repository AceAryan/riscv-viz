#pragma once
#include <cstdint>
#include <array>

class RegisterFile {
private:
    std::array<int32_t, 32> regs;

public:
    RegisterFile();
    int32_t read(int index);
    void write(int index, int32_t value);
    void reset();
    void print();  // useful for debugging in terminal
};