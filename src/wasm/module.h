#pragma once
#include "code_section.h"
#include "function_section.h"
#include "import_section.h"
#include "start_section.h"
#include "type_section.h"
#include <stdio.h>

typedef struct WasmModule {
  WasmTypeSection *typesSection;
  WasmImportSection *importSection;
  WasmFunctionSection *functionSection;
  WasmStartSection *startSection;
  WasmCodeSection *codeSection;
} WasmModule;

WasmModule *createModule();
void fprintfWasmModule(FILE *channel, WasmModule *module);
void printfWasmModule(WasmModule *module);
void freeWasmModule(WasmModule *module);

void writeModule(FILE *file, WasmModule *module);