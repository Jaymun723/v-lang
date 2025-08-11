#pragma once
#include "ast/program.h"
#include "charvec.h"
#include "wasm/module.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * Does not own anything.
 */
typedef struct FuncMapper FuncMapper;

FuncMapper *createFuncMapper();
FuncMapper *createDefaultFuncMapper();
void freeFuncMapper(FuncMapper *fm);
void completlyFreeFuncMapper(FuncMapper *fm);

void fprintfFuncMapper(FILE *channel, FuncMapper *fm);

unsigned int addFunction(FuncMapper *fm, const char *funcName, CharVec *params,
                         CharVec *locals, CharVec *results, bool import);
int getFunctionIndex(FuncMapper *fm, const char *funcName);

/**
 * Property: 0 -> params, 1 -> locals, 2 -> results
 */
CharVec *getFunctionProperty(FuncMapper *fm, const char *funcName,
                             int property);

void emitFuncMapper(WasmModule *module, FuncMapper *fm);
