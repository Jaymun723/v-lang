#include "type_section.h"
#include "../mystring.h"
#include "function_section.h"
#include <stdio.h>
#include <stdlib.h>

void freeFunctionType(WasmFunctionType *funcType) {
  // printf("freeing funcTyp -> %p\n", (void *)funcType);
  // printf("going to free params at: %p\n", (void *)funcType->params);
  freeCv(funcType->params);
  // printf("going to free results at: %p\n", (void *)funcType->results);
  freeCv(funcType->results);
  if (funcType->next != NULL) {
    freeFunctionType(funcType->next);
  }
  free(funcType);
}

void fprintfFunctionType(FILE *channel, WasmFunctionType *funcType) {
  fprintf(channel, " (type (func");
  if (funcType->params->length != 0) {
    fprintf(channel, " (param ");
    fprintfCv(channel, funcType->params, true);
    fprintf(channel, ")");
  }
  if (funcType->results->length != 0) {
    fprintf(channel, " (result ");
    fprintfCv(channel, funcType->results, true);
    fprintf(channel, ")");
  }
  fprintf(channel, "))");
  if (funcType->next != NULL) {
    fprintf(channel, "\n");
    fprintfFunctionType(channel, funcType->next);
  }
}

void addFunctionType(WasmTypesSection *section, CharVec *params,
                     CharVec *results) {
  WasmFunctionType *funcType =
      (WasmFunctionType *)malloc(sizeof(WasmFunctionType));
  funcType->id = 0x60;
  funcType->params = params;
  funcType->results = results;
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

WasmTypesSection *createDefaultTypesSection() {
  WasmTypesSection *typesSection =
      (WasmTypesSection *)malloc(sizeof(WasmTypesSection));
  typesSection->id = 0x01;
  typesSection->numTypes = 0;
  typesSection->funcTypesHead = NULL;
  typesSection->funcTypesTail = NULL;

  // () -> ()
  CharVec *paramsType0 = cvCreate();
  // printf("paramsType0 <- %p\n", (void *)paramsType0);
  CharVec *resultsType0 = cvCreate();
  // printf("resultsType0 <- %p\n", (void *)resultsType0);
  addFunctionType(typesSection, paramsType0, resultsType0);

  // i32 -> ()
  CharVec *paramsType1 = cvCreate();
  // printf("paramsType1 <- %p\n", (void *)paramsType1);
  appendCv(paramsType1, (char)wasm_f32);
  CharVec *resultsType1 = cvCreate();
  // printf("resultsType1 <- %p\n", (void *)resultsType1);
  addFunctionType(typesSection, paramsType1, resultsType1);

  return typesSection;
}

void freeWasmTypesSection(WasmTypesSection *section) {
  if (section->funcTypesHead != NULL) {
    freeFunctionType(section->funcTypesHead);
  }
  free(section);
}

void fprintfWasmTypeSection(FILE *channel, WasmTypesSection *section) {
  if (section->funcTypesHead != NULL) {
    fprintfFunctionType(channel, section->funcTypesHead);
  }
}
