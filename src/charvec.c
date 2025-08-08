#include "charvec.h"
#include "leb128.h"
#include <stdlib.h>

CharVec *cvCreate() {
  CharVec *res = (CharVec *)malloc(sizeof(CharVec));
  res->length = 0;
  res->capacity = 2;
  res->data = (char *)malloc(sizeof(char) * res->capacity);
  return res;
}

void freeCv(CharVec *charVec) {
  free(charVec->data);
  free(charVec);
}

void printfCv(CharVec *charVec) { fprintfCv(stdout, charVec, false); }

void fprintfCv(FILE *channel, CharVec *cv, bool oneLine) {
  for (unsigned int i = 0; i < cv->length; i++) {
    fprintf(channel, "0x%02hhx", cv->data[i]);
    if (i + 1 == cv->length) {
      if (!oneLine) {
        fprintf(channel, "\n");
      }
    } else {
      fprintf(channel, " ");
    }
  }
}

void appendCv(CharVec *charVec, char c) {
  charVec->data[charVec->length] = c;
  charVec->length++;
  if (charVec->length == charVec->capacity) {
    charVec->capacity = 2 * charVec->length;
    char *new_data = (char *)malloc(sizeof(char) * charVec->capacity);
    for (unsigned int i = 0; i < charVec->length; i++) {
      new_data[i] = charVec->data[i];
    }
    free(charVec->data);
    charVec->data = new_data;
  }
}

void addCv(CharVec *charVec, char *c, int n) {
  for (int i = 0; i < n; i++) {
    appendCv(charVec, c[i]);
  }
}

void setCv(CharVec *cv, unsigned int i, char c) {
  if (i >= cv->length) {
    fprintf(stderr, "setCv: indexing outside of the charVector: %d >= %d.\n", i,
            cv->length);
    return;
  }
  cv->data[i] = c;
}

char getCv(CharVec *cv, unsigned int i) {
  if (i >= cv->length) {
    fprintf(stderr, "getCv: indexing outside of the charVector: %d >= %d.\n", i,
            cv->length);
    return 0x00;
  }
  return cv->data[i];
}

void writeCv(FILE *file, CharVec *cv) {
  writeULEB128(file, cv->length);
  writeRawCv(file, cv);
}

void writeRawCv(FILE *file, CharVec *cv) {
  for (unsigned int i = 0; i < cv->length; i++) {
    fputc(cv->data[i], file);
  }
}

int sizeCv(CharVec *cv) { return sizeLEB128(cv->length) + cv->length; }
