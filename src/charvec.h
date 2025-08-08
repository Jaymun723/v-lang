#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct CharVec {
  unsigned int capacity;
  unsigned int length;
  char *data;
} CharVec;

CharVec *cvCreate();
void freeCv(CharVec *charVec);
void printfCv(CharVec *charVec);
void fprintfCv(FILE *channel, CharVec *cv, bool oneLine);

void appendCv(CharVec *charVec, char c);
void addCv(CharVec *charVec, char *c, int n);
void setCv(CharVec *cv, unsigned int i, char c);
char getCv(CharVec *cv, unsigned int i);

void writeCv(FILE *file, CharVec *c);
void writeRawCv(FILE *file, CharVec *c);
int sizeCv(CharVec *cv);