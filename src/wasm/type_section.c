#include "type_section.h"
#include "../common.h"
#include "../leb128.h"
#include "../mystring.h"
#include "const.h"
#include "function_section.h"
#include <stdio.h>
#include <stdlib.h>

void freeFunctionType(WasmFunctionType *funcType) {
  // printf("freeing funcTyp -> %p\n", (void *)funcType);
  // printf("going to free param at: %p\n", (void *)funcType->param);
  freeCv(funcType->param);
  // printf("going to free result at: %p\n", (void *)funcType->result);
  freeCv(funcType->result);
  if (funcType->next != NULL) {
    freeFunctionType(funcType->next);
  }
  free(funcType);
}

void fprintfFunctionType(FILE *channel, WasmFunctionType *funcType) {
  fprintf(channel, " (type (func");
  if (funcType->param && funcType->param->length != 0) {
    fprintf(channel, " (param ");
    fprintfCustomCv(channel, funcType->param, fprintfWasmConst);
    fprintf(channel, ")");
  }
  if (funcType->result && funcType->result->length != 0) {
    fprintf(channel, " (result ");
    fprintfCustomCv(channel, funcType->result, fprintfWasmConst);
    fprintf(channel, ")");
  }
  fprintf(channel, "))");
  if (funcType->next != NULL) {
    fprintf(channel, "\n");
    fprintfFunctionType(channel, funcType->next);
  }
}

int addFunctionType(WasmTypeSection *section, CharVec *param, CharVec *result) {
  WasmFunctionType *funcType =
      (WasmFunctionType *)malloc(sizeof(WasmFunctionType));
  funcType->id = WASM_CONST_CODE[WasmConst_functiontype];
  funcType->param = param;
  funcType->result = result;
  funcType->next = NULL;

  if (section->numTypes == 0) {
    section->funcTypesHead = funcType;
  } else {
    section->funcTypesTail->next = funcType;
  }
  section->funcTypesTail = funcType;
  section->numTypes++;

  return section->numTypes - 1;
}

WasmTypeSection *createDefaultTypeSection() {
  WasmTypeSection *typeSection =
      (WasmTypeSection *)malloc(sizeof(WasmTypeSection));
  typeSection->id = 0x01;
  typeSection->numTypes = 0;
  typeSection->funcTypesHead = NULL;
  typeSection->funcTypesTail = NULL;

  return typeSection;
}

void freeWasmTypeSection(WasmTypeSection *section) {
  if (section->funcTypesHead != NULL) {
    freeFunctionType(section->funcTypesHead);
  }
  free(section);
}

void fprintfWasmTypeSection(FILE *channel, WasmTypeSection *section) {
  if (section->funcTypesHead != NULL) {
    fprintfFunctionType(channel, section->funcTypesHead);
  }
}

int sizeWasmFunctionType(WasmFunctionType *funcType) {
  int size = 2; // at least num param, num result
  size += funcType->param ? funcType->param->length : 0;
  size += funcType->result ? funcType->result->length : 0;
  return size;
}

void writeFunctionType(FILE *file, WasmFunctionType *funcType) {
  fputc(funcType->id, file);
  writeCv(file, funcType->param);
  writeCv(file, funcType->result);
  if (funcType->next != NULL) {
    writeFunctionType(file, funcType->next);
  }
}

int sizeWasmTypeSection(WasmTypeSection *section) {
  int size = 1;
  for (WasmFunctionType *funcType = section->funcTypesHead; funcType != NULL;
       funcType = funcType->next) {
    size += 1 + sizeWasmFunctionType(funcType);
  }
  return size;
}

void writeWasmTypeSection(FILE *file, WasmTypeSection *section) {
  writeByte(file, section->id);
  writeULEB128(file, sizeWasmTypeSection(section));
  writeULEB128(file, section->numTypes);
  writeFunctionType(file, section->funcTypesHead);
}
