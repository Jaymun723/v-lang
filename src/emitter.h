#pragma once
#include "ast/program.h"
#include "wasm/module.h"
#include <stdio.h>

void emit(FILE *file, AstProgram *program, WasmModule *module);