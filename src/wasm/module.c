#include "module.h"
#include <stdlib.h>

WasmModule *createModule() {
  WasmModule *module = (WasmModule *)malloc(sizeof(WasmModule));
  // module->typesSection = NULL;
  module->typesSection = createDefaultTypesSection();
  module->importSection = createDefaultImportSection();
  module->functionSection = createDefaultWasmFunctionSection();
  module->startSection = createDefaultWasmStartSection();
  module->codeSection = createDefaultWasmCodeSection();
  return module;
}

void fprintfWasmModule(FILE *channel, WasmModule *module) {
  fprintf(channel, "(module\n");
  if (module->typesSection != NULL) {
    fprintfWasmTypeSection(channel, module->typesSection);
    fprintf(channel, "\n");
  }
  if (module->importSection != NULL) {
    fprintfWasmImportSection(channel, module->importSection);
    fprintf(channel, "\n");
  }
  if (module->functionSection != NULL) {
    fprintfWasmFunctionSection(channel, module->functionSection);
    fprintf(channel, "\n");
  }
  if (module->startSection != NULL) {
    fprintfWasmStartSection(channel, module->startSection);
    fprintf(channel, "\n");
  }
  if (module->codeSection != NULL) {
    fprintfWasmCodeSection(channel, module->codeSection);
    fprintf(channel, "\n");
  }
  fprintf(channel, ")\n");
}

void printfWasmModule(WasmModule *module) { fprintfWasmModule(stdout, module); }

void freeWasmModule(WasmModule *module) {
  if (module->typesSection != NULL) {
    freeWasmTypesSection(module->typesSection);
  }
  if (module->importSection != NULL) {
    freeWasmImportSection(module->importSection);
  }
  if (module->functionSection != NULL) {
    freeWasmFunctionSection(module->functionSection);
  }
  if (module->startSection != NULL) {
    freeWasmStartSection(module->startSection);
  }
  if (module->codeSection != NULL) {
    freeWasmCodeSection(module->codeSection);
  }
  free(module);
}

void writeHead(FILE *file) {
  char magic_number[] = {0x00, 0x61, 0x73, 0x6d};
  fwrite(magic_number, sizeof(char), 4, file);
  char version[] = {0x01, 0x00, 0x00, 0x00};
  fwrite(version, sizeof(char), 4, file);
}

void writeModule(FILE *file, WasmModule *module) {
  writeHead(file);
  writeWasmTypeSection(file, module->typesSection);
  writeWasmImportSection(file, module->importSection);
  writeWasmFunctionSection(file, module->functionSection);
  writeWasmStartSection(file, module->startSection);
  writeWasmCodeSection(file, module->codeSection);
}