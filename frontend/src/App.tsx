import { useState } from 'react';
import type { CPUState } from './types/index.ts';
import { loadProgram, step, stepBack, run, reset } from './api/simulator';
import RegisterViewer from './components/RegisterViewer';
import MemoryViewer from './components/MemoryViewer';
import InstructionPanel from './components/InstructionPanel';
import Controls from './components/Controls';

const EMPTY_STATE: CPUState = {
  pc: 0, cycle: 0, halted: false,
  registers: new Array(32).fill(0),
  memory: new Array(256).fill(0),
};

export default function App() {
  const [state, setState]     = useState<CPUState>(EMPTY_STATE);
  const [prevRegs, setPrevRegs] = useState<number[]>(new Array(32).fill(0));
  const [error, setError]     = useState<string>('');

  const wrap = async (fn: () => Promise<CPUState>) => {
    try {
      setError('');
      setPrevRegs(state.registers);
      const next = await fn();
      setState(next);
    } catch (e: any) {
      setError(e?.response?.data?.detail || e.message || 'Error');
    }
  };

  return (
    <div className="app">
      <header>
        <h1>riscv-viz</h1>
        <span>RISC-V CPU Simulator</span>
      </header>

      {error && <div className="error">{error}</div>}

      <div className="layout">
        <div className="left">
          <Controls
            onLoad={(p) => wrap(() => loadProgram(p))}
            onStep={() => wrap(step)}
            onStepBack={() => wrap(stepBack)}
            onRun={() => wrap(run)}
            onReset={() => wrap(reset)}
            cycle={state.cycle}
            halted={state.halted}
          />
          <InstructionPanel pc={state.pc} memory={state.memory} />
        </div>
        <div className="right">
          <RegisterViewer registers={state.registers} prevRegisters={prevRegs} />
          <MemoryViewer memory={state.memory} pc={state.pc} />
        </div>
      </div>
    </div>
  );
}