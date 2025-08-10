#pragma once
#include "ast/program.h"
#include "func_mapper.h"
#include "hashmap.h"
#include "wasm/module.h"
#include <stdbool.h>

void emit(WasmModule *module, FuncMapper *fm, AstProgram *program);