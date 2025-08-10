#include "charvec.h"
#include "common.h"
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
    writeByte(file, cv->data[i]);
  }
}

int sizeCv(CharVec *cv) { return sizeLEB128(cv->length) + cv->length; }

char *cvToString(CharVec *cv) {
  char *string = (char *)malloc(sizeof(char) * (cv->length + 1));
  string[cv->length] = '\0';
  for (unsigned int i = 0; i < cv->length; i++) {
    string[i] = cv->data[i];
  }
  freeCv(cv);
  return string;
}

int *cvToInteger(CharVec *cv) {
  int *res = malloc(sizeof(int));
  *res = 0;
  for (unsigned int i = 0; i < cv->length; i++) {
    int n = (int)(cv->data[i] - '0');
    *res *= 10;
    *res += n;
  }
  freeCv(cv);
  return res;
}

double *cvToFloatting(CharVec *cv) {
  double *res = malloc(sizeof(double));
  *res = 0.0;
  unsigned int i = 0;
  while (cv->data[i] != '.') {
    int n = (int)(cv->data[i] - '0');
    *res *= 10.0;
    *res += (double)n;
    i++;
    // printf("float (start): read %d, thus res is %f\n", n, *res);
  }
  double rest = 0.0;
  for (unsigned int j = cv->length - 1; j > i; j--) {
    int n = (int)(cv->data[j] - '0');
    rest = 0.1 * rest + n;
    // printf("float (end): read %d, thus rest is %f\n", n, rest);
  }
  *res = *res + 0.1 * rest;
  freeCv(cv);
  return res;
}
