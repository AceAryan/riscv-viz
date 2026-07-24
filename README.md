# riscv-viz

Cycle-accurate RISC-V simulator with live register, memory, and datapath visualization. C++ simulation core exposed via REST API, React frontend.

---

## Stack

- **Simulator** — C++17, CMake
- **Backend** — FastAPI (bridges C++ binary to frontend)
- **Frontend** — React, TypeScript, Vite

---

## Simulator core

Implements a subset of the RV32I base integer ISA:

| Type | Instructions |
|------|-------------|
| R | `add` `sub` `and` `or` `xor` `sll` `srl` `slt` |
| I | `addi` `andi` `ori` `lw` `jalr` |
| S | `sw` |
| B | `beq` `bne` `blt` `bge` |
| J | `jal` |

Instruction decoding uses direct bit extraction — no lookup tables. Immediates are sign-extended per the RV32I spec.

---

## API

| Endpoint | Description |
|----------|-------------|
| `POST /load` | Load assembly program into memory |
| `POST /step` | Execute one cycle, return updated state |
| `POST /run` | Run until halt or max cycles |
| `POST /reset` | Reset CPU state |
| `GET /state` | Return full register file + memory snapshot |

All responses are JSON containing `{ registers, memory, pc, current_instruction }`.

---

## Setup

```bash
# build simulator
cd simulator && mkdir build && cd build
cmake .. && make

# backend
cd backend && pip install fastapi uvicorn
uvicorn server:app --reload

# frontend
cd frontend && npm install && npm run dev
```

