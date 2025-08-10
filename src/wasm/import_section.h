#pragma once
#include <stdio.h>

typedef struct WasmImport {
  char *mod;
  char *name;
  char importType;
  unsigned int typeIndex;

  struct WasmImport *next;
} WasmImport;

typedef struct WasmImportSection {
  char id;
  unsigned int numImports;

  WasmImport *importHead;
  WasmImport *importTail;
} WasmImportSection;

void addWasmImport(WasmImportSection *section, const char *mod,
                   const char *name, char importType, char typeIndex);
WasmImportSection *createDefaultImportSection();
void fprintfWasmImportSection(FILE *channel, WasmImportSection *section);
void freeWasmImportSection(WasmImportSection *section);
void writeWasmImportSection(FILE *file, WasmImportSection *section);