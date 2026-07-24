#pragma once
#include <cstdint>
#include <vector>
#include <string>

class Parser {
public:
    // parse a .s file → vector of encoded instructions
    std::vector<uint32_t> parseFile(const std::string& filename);

    // parse a string directly — useful for frontend editor
    std::vector<uint32_t> parseText(const std::string& text);

private:
    uint32_t encodeLine(const std::string& line);

    // encoders per instruction type
    uint32_t encodeR(uint32_t funct7, int rs2, int rs1, uint32_t funct3, int rd, uint32_t opcode);
    uint32_t encodeI(int32_t imm, int rs1, uint32_t funct3, int rd, uint32_t opcode);
    uint32_t encodeS(int32_t imm, int rs2, int rs1, uint32_t funct3, uint32_t opcode);
    uint32_t encodeB(int32_t imm, int rs2, int rs1, uint32_t funct3, uint32_t opcode);
    uint32_t encodeJ(int32_t imm, int rd, uint32_t opcode);

    // helpers
    int parseReg(const std::string& reg);   // "x1" → 1
    int32_t parseImm(const std::string& s); // "10" or "-5" → integer
    std::string trim(const std::string& s);
    std::vector<std::string> tokenize(const std::string& line);
};