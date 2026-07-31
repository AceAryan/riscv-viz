export interface CPUState {
  pc: number;
  cycle: number;
  halted: boolean;
  registers: number[];
  memory: number[];
}