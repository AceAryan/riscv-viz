interface Props {
  registers: number[];
  prevRegisters: number[];
}

const ABI = [
  'zero','ra','sp','gp','tp',
  't0','t1','t2',
  's0','s1',
  'a0','a1','a2','a3','a4','a5','a6','a7',
  's2','s3','s4','s5','s6','s7','s8','s9','s10','s11',
  't3','t4','t5','t6'
];

export default function RegisterViewer({ registers, prevRegisters }: Props) {
  return (
    <div className="panel">
      <h3>Registers</h3>
      <div className="reg-grid">
        {registers.map((val, i) => {
          const changed = val !== prevRegisters[i];
          return (
            <div key={i} className={`reg-row ${changed ? 'changed' : ''}`}>
              <span className="reg-name">x{i}</span>
              <span className="reg-abi">{ABI[i]}</span>
              <span className="reg-val">{val}</span>
              <span className="reg-hex">0x{val.toString(16).padStart(8, '0')}</span>
            </div>
          );
        })}
      </div>
    </div>
  );
}