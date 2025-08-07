#pragma once
#include "import_section.h"
#include "type_section.h"
#include <stdio.h>

typedef struct WasmModule {
  WasmTypesSection *typesSection;
  WasmImportSection *importSection;
} WasmModule;

WasmModule *createModule();
void fprintfWasmModule(FILE *channel, WasmModule *module);
void printfWasmModule(WasmModule *module);
void freeWasmModule(WasmModule *module);