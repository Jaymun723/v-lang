#include "code_section.h"
#include "../common.h"
#include "../leb128.h"
#include "const.h"
#include <stdlib.h>

void addWasmCode(WasmCodeSection *section, CharVec *locals, CharVec *body) {
  WasmCode *code = (WasmCode *)malloc(sizeof(WasmCode));
  code->locals = locals;
  code->body = body;
  code->next = NULL;

  if (section->numCodes == 0) {
    section->codeHead = code;
  } else {
    section->codeTail->next = code;
  }
  section->codeTail = code;
  section->numCodes++;
}

void fprintfWasmCode(FILE *channel, WasmCode *code) {
  fprintf(channel, " (func\n  (;locals;)");
  fprintfCustomCv(channel, code->locals, fprintfWasmConst);

  fprintf(channel, "\n  (;body;) ");
  fprintfCustomCv(channel, code->body, fprintfWasmConst);

  fprintf(channel, ")");

  if (code->next != NULL) {
    fprintf(channel, "\n");
    fprintfWasmCode(channel, code->next);
  }
}

void freeWasmCode(WasmCode *code) {
  freeCv(code->locals);
  freeCv(code->body);
  if (code->next != NULL) {
    freeWasmCode(code->next);
  }
  free(code);
}

WasmCodeSection *createDefaultWasmCodeSection() {
  WasmCodeSection *section = (WasmCodeSection *)malloc(sizeof(WasmCodeSection));

  section->id = 0x0a;
  section->numCodes = 0;
  section->codeHead = NULL;
  section->codeTail = NULL;

  CharVec *locals = cvCreate();
  CharVec *body = cvCreate();
  // appendCv(body, 0x41);
  // appendCv(body, 0x07);
  // appendCv(body, 0x10);
  // appendCv(body, 0x00);
  // appendCv(body, 0x0b);
  addWasmCode(section, locals, body);

  return section;
}

void fprintfWasmCodeSection(FILE *channel, WasmCodeSection *section) {
  if (section->codeHead != NULL) {
    fprintfWasmCode(channel, section->codeHead);
  }
}

void freeWasmCodeSection(WasmCodeSection *section) {
  if (section->codeHead != NULL) {
    freeWasmCode(section->codeHead);
  }
  free(section);
}

int sizeWasmCode(WasmCode *code) {
  return sizeCv(code->locals) + code->body->length;
}

void writeWasmCode(FILE *file, WasmCode *code) {
  writeULEB128(file, sizeWasmCode(code));
  writeCv(file, code->locals);
  writeRawCv(file, code->body);
  if (code->next != NULL) {
    writeWasmCode(file, code->next);
  }
}

int sizeWasmCodeSection(WasmCodeSection *section) {
  int size = 1;
  for (WasmCode *code = section->codeHead; code != NULL; code = code->next) {
    size += 1 + sizeWasmCode(code);
  }
  return size;
}

void writeWasmCodeSection(FILE *file, WasmCodeSection *section) {
  writeByte(file, section->id);
  writeULEB128(file, sizeWasmCodeSection(section));
  writeULEB128(file, section->numCodes);
  if (section->numCodes != 0) {
    writeWasmCode(file, section->codeHead);
  }
}