# riscv-viz

Cycle-accurate RISC-V simulator with live register, memory, and datapath visualization. C++ simulation core exposed via REST API, React frontend.

**[Live Demo](https://riscv-viz.vercel.app)**

---

## Demo

![demo](demo.gif)

---

## Stack

- **Simulator** — C++17, CMake
- **Backend** — FastAPI + Docker (bridges C++ binary to frontend)
- **Frontend** — React, TypeScript, Vite
- **Deployment** — Vercel (frontend), Render (backend)

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

Instruction decoding uses direct bit extraction — no lookup tables. Immediates are sign-extended per the RV32I spec. Memory is byte-addressable with little-endian load/store and alignment checks.

---

## API

| Endpoint | Description |
|----------|-------------|
| `POST /load` | Load assembly program into memory |
| `POST /step` | Execute one cycle, return updated state |
| `POST /stepback` | Rewind one cycle |
| `POST /run` | Run until halt or max cycles |
| `POST /reset` | Reset CPU state |
| `GET /state` | Return full register file + memory snapshot |

All responses are JSON: `{ pc, cycle, halted, registers, memory }`.

---

## Local setup

### Simulator

```bash
cd simulator
mkdir build && cd build
cmake .. && make
./riscv-sim ../tests/fibonacci.s
```

### Backend

```bash
cd backend
python3 -m venv venv && source venv/bin/activate
pip install fastapi uvicorn pydantic
uvicorn server:app --reload
```

### Frontend

```bash
cd frontend
npm install
npm run dev
```

### Docker (recommended)

```bash
docker build -f backend/Dockerfile -t riscv-viz-backend .
docker run -p 8000:8000 riscv-viz-backend
```

