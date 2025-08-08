#pragma once
#include <stdio.h>

typedef struct WasmImport {
  char *mod;
  char *name;
  char importType;
  unsigned int index;

  struct WasmImport *next;
} WasmImport;

typedef struct WasmImportSection {
  char id;
  unsigned int numImports;

  WasmImport *importHead;
  WasmImport *importTail;
} WasmImportSection;

WasmImportSection *createDefaultImportSection();
void fprintfWasmImportSection(FILE *channel, WasmImportSection *section);
void freeWasmImportSection(WasmImportSection *section);
void writeWasmImportSection(FILE *file, WasmImportSection *section);