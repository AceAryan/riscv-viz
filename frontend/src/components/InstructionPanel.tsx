interface Props {
  pc: number;
  memory: number[];
}

export default function InstructionPanel({ pc, memory }: Props) {
  const instructions = [];
  for (let i = 0; i < 48; i += 4) {
    const word = memory[i] | (memory[i+1] << 8) | (memory[i+2] << 16) | (memory[i+3] << 24);
    if (word === 0) break;
    instructions.push({ addr: i, word });
  }
  return (
    <div className="panel">
      <h3>Instructions</h3>
      <div className="instr-list">
        {instructions.map(({ addr, word }) => (
          <div key={addr} className={`instr-row ${addr === pc ? 'current-instr' : ''}`}>
            <span className="instr-addr">0x{addr.toString(16).padStart(4, '0')}</span>
            <span className="instr-word">0x{(word >>> 0).toString(16).padStart(8, '0')}</span>
          </div>
        ))}
      </div>
    </div>
  );
}
