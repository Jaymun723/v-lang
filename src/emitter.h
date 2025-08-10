#pragma once
#include "ast/program.h"
#include "hashmap.h"
#include "wasm/module.h"
#include <stdbool.h>

typedef struct FuncDesc {
  unsigned int index;
  unsigned int typeIndex;
  bool import;
} FuncDesc;
typedef HashMap FuncMapper;

void funcMapperSet(FuncMapper *fm, const char *funcName, unsigned int index,
                   unsigned int typeIndex, bool import);
FuncDesc *funcMapperGet(FuncMapper *fm, const char *funcName);
void freeFuncMapper(FuncMapper *fm);
void printfFuncMapper(FuncMapper *fm);

void emit(WasmModule *module, AstProgram *program);