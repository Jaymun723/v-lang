#include "import_section.h"
#include "../mystring.h"
#include <stdio.h>
#include <stdlib.h>

void addWasmImport(WasmImportSection *section, const char *mod,
                   const char *name, char importType, char index) {
  WasmImport *import = (WasmImport *)malloc(sizeof(WasmImport));
  import->mod = stringCopyN(mod, -1);
  import->name = stringCopyN(name, -1);
  import->importType = importType;
  import->index = index;
  import->next = NULL;

  if (section->numImports == 0) {
    section->importHead = import;
  } else {
    section->importTail->next = import;
  }
  section->importTail = import;
  section->numImports++;
}

void fprintfWasmImport(FILE *channel, WasmImport *import) {
  fprintf(channel, " (import \"%s\" \"%s\" ", import->mod, import->name);
  if (import->importType == 0x00) {
    fprintf(channel, "(func (type %d))", import->index);
  }
  fprintf(channel, ")");
  if (import->next != NULL) {
    fprintf(channel, "\n");
    fprintfWasmImport(channel, import->next);
  }
}

void freeWasmImport(WasmImport *import) {
  free(import->mod);
  free(import->name);
  if (import->next != NULL) {
    freeWasmImport(import->next);
  }
  free(import);
}

WasmImportSection *createDefaultImportSection() {
  WasmImportSection *section =
      (WasmImportSection *)malloc(sizeof(WasmImportSection));
  section->id = 0x02;
  section->numImports = 0;
  section->importHead = NULL;
  section->importTail = NULL;

  addWasmImport(section, "env", "print", 0x00, 1);

  return section;
}

void fprintfWasmImportSection(FILE *channel, WasmImportSection *section) {
  if (section->importHead != NULL) {
    fprintfWasmImport(channel, section->importHead);
  }
}

void freeWasmImportSection(WasmImportSection *section) {
  if (section->importHead != NULL) {
    freeWasmImport(section->importHead);
  }
  free(section);
}

int sizeWasmImport(WasmImport *import) {
  int size = 2;
  size += 1 + stringLength(import->mod);
  size += 1 + stringLength(import->name);
  return size;
}

void writeWasmImport(FILE *file, WasmImport *import) {
  writeString(file, import->mod);
  writeString(file, import->name);
  fputc(import->importType, file);
  fputc(import->index, file);

  if (import->next != NULL) {
    writeWasmImport(file, import->next);
  }
}

int sizeWasmImportSection(WasmImportSection *section) {
  int size = 1;
  for (WasmImport *import = section->importHead; import != NULL;
       import = import->next) {
    size += sizeWasmImport(import);
  }
  return size;
}

void writeWasmImportSection(FILE *file, WasmImportSection *section) {
  fputc(section->id, file);
  int size = sizeWasmImportSection(section);
  // printf("sizeWasmImportSection=%d\n", size);
  fputc(size, file);
  fputc(section->numImports, file);
  if (section->numImports != 0) {
    writeWasmImport(file, section->importHead);
  }
}