#pragma once
#include <stdbool.h>

int stringLength(const char *string);

int stringCmp(const char *a, const char *b);

/**
 * Returns a copy of the n first chars of source, whole string is copied if n ==
 * -1.
 */
char *stringCopyN(const char *source, int n);

bool isNum(char c);
bool isAlpha(char c);
bool isAlphanum(char c);

bool isInteger(const char *str);
bool isFloating(const char *str);