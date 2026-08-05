import subprocess
import json
import os
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI()

# allow React frontend to call this API
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# path to compiled simulator binary
BINARY = "/mnt/d/Dev/CPUsim/simulator/build/riscv-sim"  

# ── in-memory CPU state ────────────────────────────
# we re-run the simulation from scratch to reach desired state
# simple but effective for a visualizer

class SimulatorState:
    def __init__(self):
        self.program_text = ""
        self.cycle = 0
        self.history = []  # list of states at each cycle

state = SimulatorState()

# ── request models ─────────────────────────────────

class LoadRequest(BaseModel):
    program: str  # raw assembly text

class StepRequest(BaseModel):
    cycle: int    # which cycle to go to

# ── helper ─────────────────────────────────────────

def run_simulator(program: str, cycles: int) -> dict:
    """Write program to temp file, run simulator, parse output"""
    import tempfile

    with tempfile.NamedTemporaryFile(mode='w', suffix='.s', delete=False) as f:
        f.write(program)
        tmpfile = f.name

    try:
        result = subprocess.run(
            [BINARY, tmpfile, str(cycles)],
            capture_output=True,
            text=True,
            timeout=5
        )
        if result.returncode != 0:
            raise HTTPException(status_code=400, detail=result.stderr)

        return json.loads(result.stdout)

    except subprocess.TimeoutExpired:
        raise HTTPException(status_code=408, detail="Simulation timed out")
    finally:
        os.unlink(tmpfile)

# ── endpoints ──────────────────────────────────────

@app.post("/load")
def load(req: LoadRequest):
    """Load a new assembly program"""
    state.program_text = req.program
    state.cycle = 0

    # run for 0 cycles to get initial state
    result = run_simulator(req.program, 0)
    state.history = [result]
    return result

@app.post("/step")
def step():
    """Advance one cycle"""
    state.cycle += 1
    result = run_simulator(state.program_text, state.cycle)
    return result

@app.post("/stepback")
def stepback():
    """Go back one cycle"""
    if state.cycle == 0:
        raise HTTPException(status_code=400, detail="Already at cycle 0")
    state.cycle -= 1
    result = run_simulator(state.program_text, state.cycle)
    return result

@app.post("/run")
def run():
    """Run to completion"""
    result = run_simulator(state.program_text, 99999)
    state.cycle = result["cycle"]
    return result

@app.post("/reset")
def reset():
    """Reset to cycle 0"""
    state.cycle = 0
    result = run_simulator(state.program_text, 0)
    return result

@app.get("/state")
def get_state():
    """Return current state"""
    if not state.program_text:
        raise HTTPException(status_code=400, detail="No program loaded")
    return run_simulator(state.program_text, state.cycle)