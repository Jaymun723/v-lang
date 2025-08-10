#include "func_mapper.h"
#include "mystring.h"
#include "wasm/const.h"
#include <stdlib.h>

typedef struct MappedFunc {
  const char *funcName;
  CharVec *params;
  CharVec *locals;
  CharVec *results;
} MappedFunc;

typedef struct FuncMapper {
  unsigned int numImported;
  unsigned int capacityImported;

  MappedFunc *importedFunctions;

  unsigned int numModule;
  unsigned int capacityModule;

  MappedFunc *moduleFunctions;
} FuncMapper;

FuncMapper *createFuncMapper() {
  FuncMapper *fm = (FuncMapper *)malloc(sizeof(FuncMapper));
  fm->numImported = 0;
  fm->capacityImported = 5;
  fm->importedFunctions =
      (MappedFunc *)malloc(sizeof(MappedFunc) * fm->capacityImported);

  fm->numModule = 0;
  fm->capacityModule = 5;
  fm->moduleFunctions =
      (MappedFunc *)malloc(sizeof(MappedFunc) * fm->capacityImported);

  return fm;
}

FuncMapper *createDefaultFuncMapper() {
  FuncMapper *fm = createFuncMapper();

  CharVec *printI32Params = cvCreate();
  appendCv(printI32Params, WASM_CONST_CODE[WasmConst_i32]);
  addFunction(fm, "print_i32", printI32Params, NULL, NULL, true);

  CharVec *printF64Params = cvCreate();
  appendCv(printF64Params, WASM_CONST_CODE[WasmConst_f64]);
  addFunction(fm, "print_f64", printF64Params, NULL, NULL, true);

  addFunction(fm, "start", NULL, NULL, NULL, false);

  return fm;
}

void freeFuncMapper(FuncMapper *fm) {
  free(fm->importedFunctions);
  free(fm->moduleFunctions);
  free(fm);
}

void completlyFreeFuncMapper(FuncMapper *fm) {
  for (unsigned int i = 0; i < fm->numImported; i++) {
    freeCv(fm->importedFunctions[i].locals);
    freeCv(fm->importedFunctions[i].params);
    freeCv(fm->importedFunctions[i].results);
  }
  free(fm->importedFunctions);
  for (unsigned int i = 0; i < fm->numModule; i++) {
    freeCv(fm->moduleFunctions[i].locals);
    freeCv(fm->moduleFunctions[i].params);
    freeCv(fm->moduleFunctions[i].results);
  }
  free(fm->moduleFunctions);
  free(fm);
}

void fprintfFuncMapper(FILE *channel, FuncMapper *fm) {
  fprintf(channel, "Imported: ");
  for (unsigned int i = 0; i < fm->numImported; i++) {
    fprintf(channel, "%s, params: ", fm->importedFunctions[i].funcName);
    fprintfCustomCv(channel, fm->importedFunctions[i].params, fprintfWasmConst);
    fprintf(channel, ", locals: ");
    fprintfCustomCv(channel, fm->importedFunctions[i].locals, fprintfWasmConst);
    fprintf(channel, ", results: ");
    fprintfCustomCv(channel, fm->importedFunctions[i].results,
                    fprintfWasmConst);
    fprintf(channel, "\n");
  }
  fprintf(channel, "Module: ");
  for (unsigned int i = 0; i < fm->numModule; i++) {
    fprintf(channel, "%s, params: ", fm->moduleFunctions[i].funcName);
    fprintfCustomCv(channel, fm->moduleFunctions[i].params, fprintfWasmConst);
    fprintf(channel, ", locals: ");
    fprintfCustomCv(channel, fm->moduleFunctions[i].locals, fprintfWasmConst);
    fprintf(channel, ", results: ");
    fprintfCustomCv(channel, fm->moduleFunctions[i].results, fprintfWasmConst);
    fprintf(channel, "\n");
  }
}

unsigned int addFunction(FuncMapper *fm, const char *funcName, CharVec *params,
                         CharVec *locals, CharVec *results, bool import) {
  if (import) {
    if (fm->numImported == fm->capacityImported) {
      fm->capacityImported *= 2;
      MappedFunc *newImportedFunctions =
          (MappedFunc *)malloc(sizeof(MappedFunc) * fm->capacityImported);
      for (unsigned int i = 0; i < fm->numImported; i++) {
        newImportedFunctions[i] = fm->importedFunctions[i];
      }
      free(fm->importedFunctions);
      fm->importedFunctions = newImportedFunctions;
    }
    fm->importedFunctions[fm->numImported].funcName = funcName;
    fm->importedFunctions[fm->numImported].params = params;
    fm->importedFunctions[fm->numImported].locals = locals;
    fm->importedFunctions[fm->numImported].results = results;
    fm->numImported++;
    return fm->numImported - 1;
  } else {
    if (fm->numModule == fm->capacityModule) {
      fm->capacityModule *= 2;
      MappedFunc *newModuleFunctions =
          (MappedFunc *)malloc(sizeof(MappedFunc) * fm->capacityModule);
      for (unsigned int i = 0; i < fm->numModule; i++) {
        newModuleFunctions[i] = fm->moduleFunctions[i];
      }
      free(fm->moduleFunctions);
      fm->moduleFunctions = newModuleFunctions;
    }
    fm->moduleFunctions[fm->numModule].funcName = funcName;
    fm->moduleFunctions[fm->numModule].params = params;
    fm->moduleFunctions[fm->numModule].locals = locals;
    fm->moduleFunctions[fm->numModule].results = results;
    fm->numModule++;
    return fm->numModule - 1;
  }
}

int getFunctionIndex(FuncMapper *fm, const char *funcName) {
  for (unsigned int i = 0; i < fm->numImported; i++) {
    if (stringCmp(fm->importedFunctions[i].funcName, funcName) == 0) {
      return i;
    }
  }
  for (unsigned int i = 0; i < fm->numModule; i++) {
    if (stringCmp(fm->moduleFunctions[i].funcName, funcName) == 0) {
      return i + fm->numImported;
    }
  }
  return -1;
}

void emitFuncMapper(WasmModule *module, FuncMapper *fm) {
  for (unsigned int i = 0; i < fm->numImported; i++) {
    int typeIndex =
        addFunctionType(module->typesSection, fm->importedFunctions[i].params,
                        fm->importedFunctions[i].results);
    addWasmImport(module->importSection, "env",
                  fm->importedFunctions[i].funcName, 0x00, typeIndex);
  }
  for (unsigned int i = 0; i < fm->numModule; i++) {
    int typeIndex =
        addFunctionType(module->typesSection, fm->moduleFunctions[i].params,
                        fm->moduleFunctions[i].results);
    registerFunctionTypeIndex(module->functionSection, typeIndex);
  }
}
