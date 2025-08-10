#!/usr/bin/env node
import fs from 'node:fs/promises';

const wasmBuffer = await fs.readFile("./out/main.wasm");

console.log("Start of the V-Lang program:")
await WebAssembly.instantiate(wasmBuffer, {
  env: {
    print_i32: console.log,
    print_f64: console.log
  }
});