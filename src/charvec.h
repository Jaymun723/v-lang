#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct CharVec {
  int capacity;
  int length;
  char *data;
} CharVec;

CharVec *cvCreate();
void freeCv(CharVec *charVec);
void cvFprintf(FILE *channel, CharVec *charVec);
void cvPrint(CharVec *charVec);
void fprintfCv(FILE *channel, CharVec *cv, bool oneLine);

void appendCv(CharVec *charVec, char c);
void addCv(CharVec *charVec, char *c, int n);
void setCv(CharVec *cv, int i, char c);
char getCv(CharVec *cv, int i);

void writeCv(FILE *file, CharVec *c);