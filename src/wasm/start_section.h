#pragma once
#include <stdio.h>

typedef struct WasmStartSection {
  char id;
  unsigned int index;
} WasmStartSection;

WasmStartSection *createDefaultWasmStartSection();
void freeWasmStartSection(WasmStartSection *section);
void fprintfWasmStartSection(FILE *channel, WasmStartSection *section);

void writeWasmStartSection(FILE *file, WasmStartSection *section);