#pragma once
#include "ast/program.h"
#include "hashmap.h"
#include "wasm/module.h"
#include <stdbool.h>

void emit(WasmModule *module, AstProgram *program);