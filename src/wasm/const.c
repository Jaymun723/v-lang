#include "const.h"

const char *WASM_CONST_STRING[] = {
    FOREACH_WASM_CONST(GENERATE_WASM_CONST_STRING)};

const unsigned char WASM_CONST_CODE[] = {
    FOREACH_WASM_CONST(GENERATE_WASM_CONST_CODE)};

void fprintfWasmConst(FILE *channel, unsigned char code) {
  for (int i = 0; i < WasmConstCount; i++) {
    if (WASM_CONST_CODE[i] == code) {
      fprintf(channel, "%s", WASM_CONST_STRING[i]);
      return;
    }
  }
  fprintf(channel, "0x%02hhx", code);
}