import axios from 'axios';
import type { CPUState } from '../types/index.ts';

const BASE = 'http://localhost:8000';

export const loadProgram = async (program: string): Promise<CPUState> => {
  const res = await axios.post(`${BASE}/load`, { program });
  return res.data;
};

export const step = async (): Promise<CPUState> => {
  const res = await axios.post(`${BASE}/step`);
  return res.data;
};

export const stepBack = async (): Promise<CPUState> => {
  const res = await axios.post(`${BASE}/stepback`);
  return res.data;
};

export const run = async (): Promise<CPUState> => {
  const res = await axios.post(`${BASE}/run`);
  return res.data;
};

export const reset = async (): Promise<CPUState> => {
  const res = await axios.post(`${BASE}/reset`);
  return res.data;
};