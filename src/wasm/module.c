#include "module.h"
#include <stdlib.h>

WasmModule *createModule() {
  WasmModule *module = (WasmModule *)malloc(sizeof(WasmModule));
  // module->typesSection = NULL;
  module->typesSection = createDefaultTypesSection();
  module->importSection = createDefaultImportSection();
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
  free(module);
}