#pragma once
#include "../charvec.h"
#include <stdio.h>

typedef struct WasmFunctionSection {
  char id;
  CharVec *types;
} WasmFunctionSection;

WasmFunctionSection *createDefaultWasmFunctionSection();
void freeWasmFunctionSection(WasmFunctionSection *section);
void fprintfWasmFunctionSection(FILE *channel, WasmFunctionSection *section);