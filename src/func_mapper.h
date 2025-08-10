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
void freeFuncMapper(FuncMapper *fm);

void fprintfFuncMapper(FILE *channel, FuncMapper *fm);

unsigned int addFunction(FuncMapper *fm, const char *funcName, CharVec *params,
                         CharVec *locals, CharVec *results, bool import);
int getFunctionIndex(FuncMapper *fm, const char *funcName);
void emitFuncMapper(WasmModule *module, FuncMapper *fm);
