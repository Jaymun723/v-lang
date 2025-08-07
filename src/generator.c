#include "generator.h"
#include "mystring.h"
#include <stdio.h>

void writeHead(FILE *file) {
  char magic_number[] = {0x00, 0x61, 0x73, 0x6d};
  fwrite(magic_number, sizeof(char), 4, file);
  char version[] = {0x01, 0x00, 0x00, 0x00};
  fwrite(version, sizeof(char), 4, file);
}

void writeFunctionType(FILE *file, WasmFunctionType *funcType) {
  fputc(funcType->id, file);
  writeCv(file, funcType->params);
  writeCv(file, funcType->results);
  if (funcType->next != NULL) {
    writeFunctionType(file, funcType->next);
  }
}

int sizeWasmFunctionType(WasmFunctionType *funcType) {
  int size = 2; // at least num params, num results
  size += funcType->params->length;
  size += funcType->results->length;
  return size;
}

int sizeWasmTypesSection(WasmTypesSection *section) {
  int size = 1;
  for (WasmFunctionType *funcType = section->funcTypesHead; funcType != NULL;
       funcType = funcType->next) {
    size += 1 + sizeWasmFunctionType(funcType);
  }
  return size;
}

void writeWasmTypeSection(FILE *file, WasmTypesSection *section) {
  fputc(section->id, file);
  fputc(sizeWasmTypesSection(section), file);
  fputc(section->numTypes, file);
  writeFunctionType(file, section->funcTypesHead);
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

int sizeWasmFunctionSection(WasmFunctionSection *section) {
  return sizeCv(section->types);
}

void writeWasmFunctionSection(FILE *file, WasmFunctionSection *section) {
  fputc(section->id, file);
  fputc(sizeWasmFunctionSection(section), file);
  writeCv(file, section->types);
}

void writeWasmStartSection(FILE *file, WasmStartSection *section) {
  fputc(section->id, file);
  fputc(0x01, file);
  fputc(section->index, file);
}

void writeModule(FILE *file, WasmModule *module) {
  writeHead(file);
  writeWasmTypeSection(file, module->typesSection);
  writeWasmImportSection(file, module->importSection);
  writeWasmFunctionSection(file, module->functionSection);
  writeWasmStartSection(file, module->startSection);
}