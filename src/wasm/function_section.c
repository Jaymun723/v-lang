#include "function_section.h"
#include "../common.h"
#include "../leb128.h"
#include <stdlib.h>

void registerFunctionTypeIndex(WasmFunctionSection *section,
                               unsigned int typeIndex) {
  // printf("Registering %d to %d.\n", section->numTypeindexes, typeIndex);
  if (section->numTypeindexes == section->capacity) {
    section->capacity *= 2;
    unsigned int *newTypeIndexes =
        (unsigned int *)malloc(sizeof(unsigned int) * section->capacity);
    for (unsigned int i = 0; i < section->numTypeindexes; i++) {
      printf("[%d] <- %d", i, section->typeIndexes[i]);
      newTypeIndexes[i] = section->typeIndexes[i];
    }
    free(section->typeIndexes);
    section->typeIndexes = newTypeIndexes;
  }
  section->typeIndexes[section->numTypeindexes] = typeIndex;
  section->numTypeindexes++;
}

WasmFunctionSection *createDefaultWasmFunctionSection() {
  WasmFunctionSection *section =
      (WasmFunctionSection *)malloc(sizeof(WasmFunctionSection));

  section->id = 0x03;
  section->numTypeindexes = 0;
  section->capacity = 2;
  section->typeIndexes =
      (unsigned int *)malloc(sizeof(unsigned int) * section->capacity);

  return section;
}

void freeWasmFunctionSection(WasmFunctionSection *section) {
  free(section->typeIndexes);
  free(section);
}
void fprintfWasmFunctionSection(FILE *channel, WasmFunctionSection *section) {
  fprintf(channel, " (;funsec ");
  for (unsigned int i = 0; i < section->numTypeindexes; i++) {
    fprintf(channel, "%d -> %d", i, section->typeIndexes[i]);
    if (i + 1 < section->numTypeindexes) {
      fprintf(channel, ", ");
    }
  }
  fprintf(channel, ";)");
}

int sizeWasmFunctionSection(WasmFunctionSection *section) {
  int size = sizeLEB128(section->numTypeindexes);
  for (unsigned int i = 0; i < section->numTypeindexes; i++) {
    size += sizeLEB128(section->typeIndexes[i]);
  }
  return size;
}

void writeWasmFunctionSection(FILE *file, WasmFunctionSection *section) {
  writeByte(file, section->id);
  writeULEB128(file, sizeWasmFunctionSection(section));
  writeULEB128(file, section->numTypeindexes);
  for (unsigned int i = 0; i < section->numTypeindexes; i++) {
    writeULEB128(file, section->typeIndexes[i]);
  }
}