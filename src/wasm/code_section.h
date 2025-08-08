#pragma once
#include "../charvec.h"
#include <stdio.h>

typedef struct WasmCode {
  CharVec *locals;
  CharVec *body;

  struct WasmCode *next;
} WasmCode;

typedef struct WasmCodeSection {
  char id;

  int numCodes;

  WasmCode *codeHead;
  WasmCode *codeTail;
} WasmCodeSection;

WasmCodeSection *createDefaultWasmCodeSection();
void fprintfWasmCodeSection(FILE *channel, WasmCodeSection *section);
void freeWasmCodeSection(WasmCodeSection *section);
void writeWasmCodeSection(FILE *file, WasmCodeSection *section);