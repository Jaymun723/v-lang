#include "start_section.h"
#include "../common.h"
#include "../leb128.h"
#include <stdlib.h>

WasmStartSection *createDefaultWasmStartSection() {
  WasmStartSection *section =
      (WasmStartSection *)malloc(sizeof(WasmStartSection));

  section->id = 0x08;
  section->index = 1;

  return section;
}

void freeWasmStartSection(WasmStartSection *section) { free(section); }

void fprintfWasmStartSection(FILE *channel, WasmStartSection *section) {
  fprintf(channel, " (start %d)", section->index);
}

void writeWasmStartSection(FILE *file, WasmStartSection *section) {
  writeByte(file, section->id);
  writeULEB128(file, 1);
  writeULEB128(file, section->index);
}