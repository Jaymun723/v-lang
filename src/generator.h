#pragma once
#include "charvec.h"
#include "wasm_type.h"
#include <stdio.h>

void writeModule(FILE *file, WasmModule *module);