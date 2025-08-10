#pragma once
#include "charvec.h"
#include <stdio.h>

int binaryLength(int n);
int sizeLEB128(int value);
void writeULEB128(FILE *file, unsigned int value);
void emitULEB128(CharVec *cv, unsigned int value);

void writeSLEB128(FILE *file, int value);
void emitSLEB128(CharVec *cv, int value);