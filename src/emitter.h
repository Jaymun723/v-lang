#pragma once
#include "ast/program.h"
#include "func_mapper.h"
#include "hashmap.h"
#include "wasm/module.h"
#include <stdbool.h>

void emitStatement(WasmModule *module, FuncMapper *fm, AstStatement *statement);
void emitStmtList(WasmModule *module, FuncMapper *fm,
                  AstStatementList *stmtList);
void emit(WasmModule *module, FuncMapper *fm, AstProgram *program);