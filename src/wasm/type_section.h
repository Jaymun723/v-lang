#pragma once
#include "../charvec.h"
#include <stdio.h>

typedef enum WasmValueTypeCode {
  wasm_f32 = 0x7F,
} WasmValueTypeCode;

typedef struct WasmFunctionType {
  char id;

  CharVec *param;
  CharVec *result;

  struct WasmFunctionType *next;
} WasmFunctionType;

typedef struct WasmTypeSection {
  char id;
  int numTypes;
  WasmFunctionType *funcTypesHead;
  WasmFunctionType *funcTypesTail;
} WasmTypeSection;

WasmTypeSection *createDefaultTypeSection();
void freeWasmTypeSection(WasmTypeSection *section);
void fprintfWasmTypeSection(FILE *channel, WasmTypeSection *section);
void writeWasmTypeSection(FILE *file, WasmTypeSection *section);