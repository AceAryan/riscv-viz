#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// ── public ─────────────────────────────────────────

std::vector<uint32_t> Parser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    std::string text((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    return parseText(text);
}

std::vector<uint32_t> Parser::parseText(const std::string& text) {
    std::vector<uint32_t> instructions;
    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream, line)) {
        // strip comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        line = trim(line);
        if (line.empty()) continue;  // skip blank lines

        instructions.push_back(encodeLine(line));
    }

    return instructions;
}

// ── encode line ────────────────────────────────────

uint32_t Parser::encodeLine(const std::string& line) {
    auto tokens = tokenize(line);
    if (tokens.empty())
        throw std::runtime_error("Empty instruction: " + line);

    std::string op = tokens[0];

    // R-type
    if (op == "add")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x0, parseReg(tokens[1]), 0x33);
    if (op == "sub")  return encodeR(0x20, parseReg(tokens[3]), parseReg(tokens[2]), 0x0, parseReg(tokens[1]), 0x33);
    if (op == "and")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x7, parseReg(tokens[1]), 0x33);
    if (op == "or")   return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x6, parseReg(tokens[1]), 0x33);
    if (op == "xor")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x4, parseReg(tokens[1]), 0x33);
    if (op == "sll")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x1, parseReg(tokens[1]), 0x33);
    if (op == "srl")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x5, parseReg(tokens[1]), 0x33);
    if (op == "sra")  return encodeR(0x20, parseReg(tokens[3]), parseReg(tokens[2]), 0x5, parseReg(tokens[1]), 0x33);
    if (op == "slt")  return encodeR(0x00, parseReg(tokens[3]), parseReg(tokens[2]), 0x2, parseReg(tokens[1]), 0x33);

    // I-type arithmetic
    if (op == "addi") return encodeI(parseImm(tokens[3]), parseReg(tokens[2]), 0x0, parseReg(tokens[1]), 0x13);
    if (op == "andi") return encodeI(parseImm(tokens[3]), parseReg(tokens[2]), 0x7, parseReg(tokens[1]), 0x13);
    if (op == "ori")  return encodeI(parseImm(tokens[3]), parseReg(tokens[2]), 0x6, parseReg(tokens[1]), 0x13);
    if (op == "xori") return encodeI(parseImm(tokens[3]), parseReg(tokens[2]), 0x4, parseReg(tokens[1]), 0x13);
    if (op == "slti") return encodeI(parseImm(tokens[3]), parseReg(tokens[2]), 0x2, parseReg(tokens[1]), 0x13);

    // I-type load — format: lw rd, imm(rs1)
    if (op == "lw") {
        // tokens[2] is "imm(rs1)" — split it
        size_t lp = tokens[2].find('(');
        size_t rp = tokens[2].find(')');
        int32_t imm = parseImm(tokens[2].substr(0, lp));
        int rs1 = parseReg(tokens[2].substr(lp + 1, rp - lp - 1));
        return encodeI(imm, rs1, 0x2, parseReg(tokens[1]), 0x03);
    }

    // S-type — format: sw rs2, imm(rs1)
    if (op == "sw") {
        size_t lp = tokens[2].find('(');
        size_t rp = tokens[2].find(')');
        int32_t imm = parseImm(tokens[2].substr(0, lp));
        int rs1 = parseReg(tokens[2].substr(lp + 1, rp - lp - 1));
        return encodeS(imm, parseReg(tokens[1]), rs1, 0x2, 0x23);
    }

    // B-type
    if (op == "beq") return encodeB(parseImm(tokens[3]), parseReg(tokens[2]), parseReg(tokens[1]), 0x0, 0x63);
    if (op == "bne") return encodeB(parseImm(tokens[3]), parseReg(tokens[2]), parseReg(tokens[1]), 0x1, 0x63);
    if (op == "blt") return encodeB(parseImm(tokens[3]), parseReg(tokens[2]), parseReg(tokens[1]), 0x4, 0x63);
    if (op == "bge") return encodeB(parseImm(tokens[3]), parseReg(tokens[2]), parseReg(tokens[1]), 0x5, 0x63);

    // J-type
    if (op == "jal") return encodeJ(parseImm(tokens[2]), parseReg(tokens[1]), 0x6F);

    throw std::runtime_error("Unknown instruction: " + op);
}

// ── encoders ───────────────────────────────────────

uint32_t Parser::encodeR(uint32_t funct7, int rs2, int rs1, uint32_t funct3, int rd, uint32_t opcode) {
    return (funct7 << 25) | (rs2 << 20) | (rs1 << 15) |
           (funct3 << 12) | (rd << 7)   | opcode;
}

uint32_t Parser::encodeI(int32_t imm, int rs1, uint32_t funct3, int rd, uint32_t opcode) {
    return ((imm & 0xFFF) << 20) | (rs1 << 15) |
           (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t Parser::encodeS(int32_t imm, int rs2, int rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm11_5 = (imm >> 5) & 0x7F;
    uint32_t imm4_0  =  imm       & 0x1F;
    return (imm11_5 << 25) | (rs2 << 20) | (rs1 << 15) |
           (funct3  << 12) | (imm4_0 << 7) | opcode;
}

uint32_t Parser::encodeB(int32_t imm, int rs2, int rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm12  = (imm >> 12) & 0x1;
    uint32_t imm11  = (imm >> 11) & 0x1;
    uint32_t imm10_5 = (imm >> 5) & 0x3F;
    uint32_t imm4_1  = (imm >> 1) & 0xF;
    return (imm12   << 31) | (imm10_5 << 25) | (rs2 << 20) |
           (rs1     << 15) | (funct3  << 12)  |
           (imm4_1  <<  8) | (imm11   <<  7)  | opcode;
}

uint32_t Parser::encodeJ(int32_t imm, int rd, uint32_t opcode) {
    uint32_t imm20   = (imm >> 20) & 0x1;
    uint32_t imm19_12 = (imm >> 12) & 0xFF;
    uint32_t imm11   = (imm >> 11) & 0x1;
    uint32_t imm10_1  = (imm >>  1) & 0x3FF;
    return (imm20    << 31) | (imm10_1  << 21) |
           (imm11    << 20) | (imm19_12 << 12) |
           (rd       <<  7) | opcode;
}

// ── helpers ────────────────────────────────────────

int Parser::parseReg(const std::string& reg) {
    std::string r = trim(reg);
    if (r[0] == 'x')
        return std::stoi(r.substr(1));
    throw std::runtime_error("Invalid register: " + reg);
}

int32_t Parser::parseImm(const std::string& s) {
    std::string t = trim(s);
    return (int32_t)std::stoi(t, nullptr, 0);  // handles decimal and 0x hex
}

std::string Parser::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

std::vector<std::string> Parser::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(line);

    // first token is the instruction name
    stream >> token;
    tokens.push_back(token);

    // remaining tokens are comma-separated operands
    std::string rest;
    std::getline(stream, rest);
    std::istringstream ops(rest);
    while (std::getline(ops, token, ','))
        tokens.push_back(trim(token));

    return tokens;
}