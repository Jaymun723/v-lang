#pragma once
#include "charvec.h"
#include "wasm/module.h"
#include <stdio.h>

void writeModule(FILE *file, WasmModule *module);