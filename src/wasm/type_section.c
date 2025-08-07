#include "type_section.h"
#include "../mystring.h"
#include <stdio.h>
#include <stdlib.h>

void freeFunctionType(WasmFunctionType *funcType) {
  freeCv(funcType->params);
  freeCv(funcType->results);
  if (funcType->next != NULL) {
    freeFunctionType(funcType);
  }
  free(funcType);
}

void fprintfFunctionType(FILE *channel, WasmFunctionType *funcType) {
  fprintf(channel, "(func");
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
  fprintf(channel, ")");
  if (funcType->next != NULL) {
    fprintf(channel, " ");
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
}

WasmTypesSection *createDefaultTypesSection() {
  WasmTypesSection *typesSection =
      (WasmTypesSection *)malloc(sizeof(WasmTypesSection));
  typesSection->id = 0x01;
  typesSection->numTypes = 0;
  typesSection->funcTypesHead = NULL;
  typesSection->funcTypesTail = NULL;

  // i32 -> ()
  CharVec *params = cvCreate();
  appendCv(params, (char)wasm_f32);
  CharVec *results = cvCreate();

  addFunctionType(typesSection, params, results);

  return typesSection;
}

void freeWasmTypesSection(WasmTypesSection *section) {
  if (section->funcTypesHead != NULL) {
    freeFunctionType(section->funcTypesHead);
  }
  free(section);
}

void fprintfWasmTypeSection(FILE *channel, WasmTypesSection *section) {
  fprintf(channel, " (type ");
  if (section->funcTypesHead != NULL) {
    fprintfFunctionType(channel, section->funcTypesHead);
  }
  fprintf(channel, ")");
}
