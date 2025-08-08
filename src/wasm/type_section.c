#include "type_section.h"
#include "../mystring.h"
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
  if (funcType->param->length != 0) {
    fprintf(channel, " (param ");
    fprintfCv(channel, funcType->param, true);
    fprintf(channel, ")");
  }
  if (funcType->result->length != 0) {
    fprintf(channel, " (result ");
    fprintfCv(channel, funcType->result, true);
    fprintf(channel, ")");
  }
  fprintf(channel, "))");
  if (funcType->next != NULL) {
    fprintf(channel, "\n");
    fprintfFunctionType(channel, funcType->next);
  }
}

void addFunctionType(WasmTypeSection *section, CharVec *param,
                     CharVec *result) {
  WasmFunctionType *funcType =
      (WasmFunctionType *)malloc(sizeof(WasmFunctionType));
  funcType->id = 0x60;
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

  // printf("New functionType: <- %p\n", (void *)funcType);
}

WasmTypeSection *createDefaultTypeSection() {
  WasmTypeSection *typeSection =
      (WasmTypeSection *)malloc(sizeof(WasmTypeSection));
  typeSection->id = 0x01;
  typeSection->numTypes = 0;
  typeSection->funcTypesHead = NULL;
  typeSection->funcTypesTail = NULL;

  // () -> ()
  CharVec *paramsType0 = cvCreate();
  // printf("paramsType0 <- %p\n", (void *)paramsType0);
  CharVec *resultsType0 = cvCreate();
  // printf("resultsType0 <- %p\n", (void *)resultsType0);
  addFunctionType(typeSection, paramsType0, resultsType0);

  // i32 -> ()
  CharVec *paramsType1 = cvCreate();
  // printf("paramsType1 <- %p\n", (void *)paramsType1);
  appendCv(paramsType1, (char)wasm_f32);
  CharVec *resultsType1 = cvCreate();
  // printf("resultsType1 <- %p\n", (void *)resultsType1);
  addFunctionType(typeSection, paramsType1, resultsType1);

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
  size += funcType->param->length;
  size += funcType->result->length;
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
  fputc(section->id, file);
  fputc(sizeWasmTypeSection(section), file);
  fputc(section->numTypes, file);
  writeFunctionType(file, section->funcTypesHead);
}
