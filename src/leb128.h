#pragma once
#include <stdio.h>

int binaryLength(int n);
int sizeULEB128(int value);
void writeULEB128(FILE *file, unsigned int value);

void writeSLEB128(FILE *file, int value);