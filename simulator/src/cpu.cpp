#include "cpu.h"
#include <iostream>
#include <stdexcept>

CPU::CPU() {
    reset();
}

void CPU::reset() {
    PC = 0;
    cycleCount = 0;
    state = CPUState::RUNNING;
    regs.reset();
    mem.reset();
}

void CPU::loadProgram(const uint32_t* instructions, int count) {
    mem.loadProgram(instructions, count);
}

// ── main loop ──────────────────────────────────────

void CPU::step() {
    if (state == CPUState::HALTED) {
        std::cout << "CPU is halted\n";
        return;
    }

    // fetch
    uint32_t raw = mem.loadWord(PC);

    // halt condition — all zeros means end of program
    if (raw == 0x00000000) {
        state = CPUState::HALTED;
        std::cerr << "CPU halted at PC=0x" << std::hex << PC << "\n";
        return;
    }

    // decode
    Instruction instr = Instruction::decode(raw);

    if (instr.type == InstrType::UNKNOWN) {
        throw std::runtime_error("Unknown instruction at PC=0x" + std::to_string(PC));
    }

    // execute
    PC += 4;  // increment PC before execute so branches can override it
    execute(instr);
    cycleCount++;
}

void CPU::run(int maxCycles) {
    while (state == CPUState::RUNNING && cycleCount < maxCycles)
        step();

    if (cycleCount >= maxCycles)
        std::cout << "Max cycles reached\n";
}

// ── execute ────────────────────────────────────────

void CPU::execute(const Instruction& instr) {
    ALUOp op = resolveALUOp(instr);

    switch (instr.type) {

        case InstrType::R: {
            int32_t a = regs.read(instr.rs1);
            int32_t b = regs.read(instr.rs2);
            regs.write(instr.rd, alu.execute(op, a, b));
            break;
        }

        case InstrType::I: {
            // lw is a special case — goes through memory
            if (instr.opcode == 0x03) {
                uint32_t addr = regs.read(instr.rs1) + instr.imm;
                regs.write(instr.rd, mem.loadWord(addr));
            } else {
                int32_t a = regs.read(instr.rs1);
                regs.write(instr.rd, alu.execute(op, a, instr.imm));
            }
            break;
        }

        case InstrType::S: {
            uint32_t addr = regs.read(instr.rs1) + instr.imm;
            mem.storeWord(addr, regs.read(instr.rs2));
            break;
        }

        case InstrType::B: {
            int32_t a = regs.read(instr.rs1);
            int32_t b = regs.read(instr.rs2);
            bool taken = false;

            switch (instr.funct3) {
                case 0x0: taken = (a == b);  break;  // beq
                case 0x1: taken = (a != b);  break;  // bne
                case 0x4: taken = (a <  b);  break;  // blt
                case 0x5: taken = (a >= b);  break;  // bge
            }

            if (taken)
                PC = (PC - 4) + instr.imm;  // PC-4 because we already incremented
            break;
        }

        case InstrType::J: {
            // jal — jump and link
            regs.write(instr.rd, PC);  // save return address (PC already incremented)
            PC = (PC - 4) + instr.imm;
            break;
        }

        default:
            throw std::runtime_error("Unhandled instruction type");
    }
}

// ── ALU op resolver ────────────────────────────────

ALUOp CPU::resolveALUOp(const Instruction& instr) {
    if (instr.type == InstrType::S || instr.opcode == 0x03)
        return ALUOp::PASS;  // address calculation

    switch (instr.funct3) {
        case 0x0:
            if (instr.type == InstrType::R && instr.funct7 == 0x20)
                return ALUOp::SUB;
            return ALUOp::ADD;
        case 0x1: return ALUOp::SLL;
        case 0x2: return ALUOp::SLT;
        case 0x3: return ALUOp::SLTU;
        case 0x4: return ALUOp::XOR;
        case 0x5:
            return (instr.funct7 == 0x20) ? ALUOp::SRA : ALUOp::SRL;
        case 0x6: return ALUOp::OR;
        case 0x7: return ALUOp::AND;
        default:  return ALUOp::ADD;
    }
}