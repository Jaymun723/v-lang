#pragma once
#include "charvec.h"
#include <stdio.h>

// Wasm Type Section
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

// Wasm Import Section
typedef struct WasmImport {
  char *mod;
  char *name;
  char importType;
  int index;

  struct WasmImport *next;
} WasmImport;

typedef struct WasmImportSection {
  char id;
  int numImports;

  WasmImport *importHead;
  WasmImport *importTail;
} WasmImportSection;

typedef struct WasmModule {
  WasmTypesSection *typesSection;
  WasmImportSection *importSection;
} WasmModule;

WasmModule *createModule();
void fprintfWasmModule(FILE *channel, WasmModule *module);
void printfWasmModule(WasmModule *module);
void freeWasmModule(WasmModule *module);