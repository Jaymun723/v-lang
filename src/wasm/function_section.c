#include "function_section.h"
#include <stdlib.h>

WasmFunctionSection *createDefaultWasmFunctionSection() {
  WasmFunctionSection *section =
      (WasmFunctionSection *)malloc(sizeof(WasmFunctionSection));

  section->id = 0x03;
  section->types = cvCreate();

  // Main fun
  appendCv(section->types, 0x00);

  return section;
}

void freeWasmFunctionSection(WasmFunctionSection *section) {
  freeCv(section->types);
  free(section);
}
void fprintfWasmFunctionSection(FILE *channel, WasmFunctionSection *section) {
  fprintf(channel, " [funsec: ");
  fprintfCv(channel, section->types, true);
  fprintf(channel, "]");
}

int sizeWasmFunctionSection(WasmFunctionSection *section) {
  return sizeCv(section->types);
}

void writeWasmFunctionSection(FILE *file, WasmFunctionSection *section) {
  fputc(section->id, file);
  fputc(sizeWasmFunctionSection(section), file);
  writeCv(file, section->types);
}