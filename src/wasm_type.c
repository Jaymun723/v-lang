#include "wasm_type.h"
#include "mystring.h"
#include <stdlib.h>

void freeFunctionType(WasmFunctionType *funcType) {
  freeCv(funcType->params);
  freeCv(funcType->results);
  if (funcType->next != NULL) {
    freeFunctionType(funcType);
  }
  free(funcType);
}

void fprintfFunctionType(FILE *channel, WasmFunctionType *funcType) {
  fprintf(channel, "(func");
  if (funcType->params->length != 0) {
    fprintf(channel, " (param ");
    fprintfCv(channel, funcType->params, true);
    fprintf(channel, ")");
  }
  if (funcType->results->length != 0) {
    fprintf(channel, " (result ");
    fprintfCv(channel, funcType->results, true);
    fprintf(channel, ")");
  }
  fprintf(channel, ")");
  if (funcType->next != NULL) {
    fprintf(channel, " ");
    fprintfFunctionType(channel, funcType->next);
  }
}

void addFunctionType(WasmTypesSection *section, CharVec *params,
                     CharVec *results) {
  WasmFunctionType *funcType =
      (WasmFunctionType *)malloc(sizeof(WasmFunctionType));
  funcType->id = 0x60;
  funcType->params = params;
  funcType->results = results;
  funcType->next = NULL;

  if (section->numTypes == 0) {
    section->funcTypesHead = funcType;
  } else {
    section->funcTypesTail->next = funcType;
  }
  section->funcTypesTail = funcType;
  section->numTypes++;
}

WasmTypesSection *createDefaultTypesSection() {
  WasmTypesSection *typesSection =
      (WasmTypesSection *)malloc(sizeof(WasmTypesSection));
  typesSection->id = 0x01;
  typesSection->numTypes = 0;
  typesSection->funcTypesHead = NULL;
  typesSection->funcTypesTail = NULL;

  // i32 -> ()
  CharVec *params = cvCreate();
  appendCv(params, (char)wasm_f32);
  CharVec *results = cvCreate();

  addFunctionType(typesSection, params, results);

  return typesSection;
}

void freeWasmTypesSection(WasmTypesSection *section) {
  if (section->funcTypesHead != NULL) {
    freeFunctionType(section->funcTypesHead);
  }
  free(section);
}

void fprintfWasmTypeSection(FILE *channel, WasmTypesSection *section) {
  fprintf(channel, " (type ");
  if (section->funcTypesHead != NULL) {
    fprintfFunctionType(channel, section->funcTypesHead);
  }
  fprintf(channel, ")");
}

void addWasmImport(WasmImportSection *section, const char *mod,
                   const char *name, char importType, char index) {
  WasmImport *import = (WasmImport *)malloc(sizeof(WasmImport));
  import->mod = stringCopyN(mod, -1);
  import->name = stringCopyN(name, -1);
  import->importType = importType;
  import->index = index;
  import->next = NULL;

  if (section->numImports == 0) {
    section->importHead = import;
  } else {
    section->importTail->next = import;
  }
  section->importTail = import;
  section->numImports++;
}

void fprintfWasmImport(FILE *channel, WasmImport *import) {
  fprintf(channel, " (import \"%s\" \"%s\" ", import->mod, import->name);
  if (import->importType == 0x00) {
    fprintf(channel, "(func (type %d))", import->index);
  }
  fprintf(channel, ")");
  if (import->next != NULL) {
    fprintf(channel, "\n");
    fprintfWasmImport(channel, import->next);
  }
}

void freeWasmImport(WasmImport *import) {
  free(import->mod);
  free(import->name);
  if (import->next != NULL) {
    freeWasmImport(import->next);
  }
  free(import);
}

WasmImportSection *createDefaultImportSection() {
  WasmImportSection *section =
      (WasmImportSection *)malloc(sizeof(WasmImportSection));
  section->id = 0x02;
  section->numImports = 0;
  section->importHead = NULL;
  section->importTail = NULL;

  addWasmImport(section, "env", "print", 0x00, 0);

  return section;
}

void fprintfWasmImportSection(FILE *channel, WasmImportSection *section) {
  if (section->importHead != NULL) {
    fprintfWasmImport(channel, section->importHead);
  }
}

void freeWasmImportSection(WasmImportSection *section) {
  if (section->importHead != NULL) {
    freeWasmImport(section->importHead);
  }
  free(section);
}

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