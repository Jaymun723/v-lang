#pragma once
#include "../charvec.h"
#include <stdio.h>

typedef struct WasmFunctionSection {
  char id;

  unsigned int capacity;

  unsigned int numTypeindexes;
  unsigned int *typeIndexes;
} WasmFunctionSection;

void registerFunctionTypeIndex(WasmFunctionSection *section,
                               unsigned int typeIndex);
WasmFunctionSection *createDefaultWasmFunctionSection();
void freeWasmFunctionSection(WasmFunctionSection *section);
void fprintfWasmFunctionSection(FILE *channel, WasmFunctionSection *section);
void writeWasmFunctionSection(FILE *file, WasmFunctionSection *section);