import fs from 'node:fs/promises';

const wasmBuffer = await fs.readFile("./out/main.wasm");

console.log("Start of the V-Lang program:")
await WebAssembly.instantiate(wasmBuffer, { env: { print: console.log } });