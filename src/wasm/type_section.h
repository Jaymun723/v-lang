#pragma once
#include "charvec.h"
#include <stdio.h>

typedef enum WasmValueTypeCode {
  wasm_f32 = 0x7F,
} WasmValueTypeCode;

typedef struct WasmFunctionType {
  char id;

  CharVec *params;
  CharVec *results;

  struct WasmFunctionType *next;
} WasmFunctionType;

typedef struct WasmTypesSection {
  char id;
  int numTypes;
  WasmFunctionType *funcTypesHead;
  WasmFunctionType *funcTypesTail;
} WasmTypesSection;

WasmTypesSection *createDefaultTypesSection();
void freeWasmTypesSection(WasmTypesSection *section);
void fprintfWasmTypeSection(FILE *channel, WasmTypesSection *section);