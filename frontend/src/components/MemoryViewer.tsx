interface Props {
  memory: number[];
  pc: number;
}

export default function MemoryViewer({ memory, pc }: Props) {
  // show first 64 bytes — enough for most programs
  const rows = [];
  for (let i = 0; i < 64; i += 4) {
    const word = memory[i] | (memory[i+1] << 8) | (memory[i+2] << 16) | (memory[i+3] << 24);
    const isPC = i === pc;
    rows.push(
      <div key={i} className={`mem-row ${isPC ? 'current-pc' : ''}`}>
        <span className="mem-addr">0x{i.toString(16).padStart(4, '0')}</span>
        <span className="mem-bytes">
          {[memory[i], memory[i+1], memory[i+2], memory[i+3]]
            .map(b => b.toString(16).padStart(2, '0'))
            .join(' ')}
        </span>
        <span className="mem-word">0x{(word >>> 0).toString(16).padStart(8, '0')}</span>
      </div>
    );
  }

  return (
    <div className="panel">
      <h3>Memory</h3>
      <div className="mem-grid">{rows}</div>
    </div>
  );
}