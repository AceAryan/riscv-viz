interface Props {
  onLoad: (program: string) => void;
  onStep: () => void;
  onStepBack: () => void;
  onRun: () => void;
  onReset: () => void;
  cycle: number;
  halted: boolean;
}

export default function Controls({ onLoad, onStep, onStepBack, onRun, onReset, cycle, halted }: Props) {
  const handleLoad = () => {
    const program = (document.getElementById('editor') as HTMLTextAreaElement).value;
    onLoad(program);
  };

  return (
    <div className="controls">
      <textarea
        id="editor"
        defaultValue={`addi x1, x0, 5\naddi x2, x0, 10\nadd x3, x1, x2`}
        rows={8}
        spellCheck={false}
      />
      <div className="buttons">
        <button onClick={handleLoad}>Load</button>
        <button onClick={onStepBack} disabled={cycle === 0}>◀ Back</button>
        <button onClick={onStep}     disabled={halted}>Step ▶</button>
        <button onClick={onRun}      disabled={halted}>Run ▶▶</button>
        <button onClick={onReset}>Reset</button>
      </div>
      <div className="status">
        Cycle: {cycle} {halted && <span className="halted">HALTED</span>}
      </div>
    </div>
  );
}