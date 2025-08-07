#pragma once
#include <stdbool.h>
#include <stdio.h>

int stringLength(const char *string);

int stringCmp(const char *a, const char *b);

/**
 * Returns a copy of the n first chars of source, whole string is copied if n ==
 * -1.
 */
char *stringCopyN(const char *source, int n);

bool isNum(char c);
bool isLetter(char c);
bool isAlphanum(char c);

bool isInteger(const char *str);
int *readInteger(const char *str, int start, int end);
bool isFloating(const char *str);
double *readFloating(const char *str, int start, int end);

void writeString(FILE *file, const char *str);