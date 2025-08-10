#include "mystring.h"
#include "common.h"
#include "leb128.h"
#include <stdio.h>
#include <stdlib.h>

int stringLength(const char *string) {
  int length = 0;
  while (string && string[length] != '\0') {
    length++;
  }

  return length;
}

int stringCmp(const char *a, const char *b) {
  if (a == NULL) {
    if (b == NULL) {
      return 0;
    } else {
      return -1;
    }
  } else if (b == NULL) {
    return 1;
  }

  int i = 0;
  while (a[i] != '\0') {
    if (b[i] == '\0' || a[i] > b[i]) {
      return 1;
    } else if (a[i] < b[i]) {
      return -1;
    }
    i++;
  }
  if (b[i] == '\0') {
    return 0;
  } else {
    return -1;
  }
}

char *stringCopyN(const char *source, int n) {
  if (n == -1) {
    n = stringLength(source);
  }
  char *res = (char *)malloc(sizeof(char) * (n + 1));
  for (int i = 0; i < n; i++) {
    res[i] = source[i];
  }
  res[n] = '\0';
  return res;
}

bool isNum(char c) { return c >= '0' && c <= '9'; }

bool isLetter(char c) {
  if (c == '_')
    return true;
  if (c >= 'a' && c <= 'z')
    return true;
  if (c >= 'A' && c <= 'Z')
    return true;
  return false;
}

bool isAlphanum(char c) { return isNum(c) || isLetter(c); }

bool isInteger(const char *str) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] < '0' || str[i] > '9') {
      return false;
    }
    i++;
  }
  if (i == 0) {
    return false;
  }
  return true;
}

int *readInteger(const char *str, int start, int end) {
  int *res = malloc(sizeof(int));
  *res = 0;
  for (int i = start; i < end; i++) {
    int n = (int)(str[i] - '0');
    // *res = 10 * (*res) + n;
    *res *= 10;
    *res += n;
    // printf("int: read %d, thus res is %d\n", n, *res);
  }
  return res;
}

bool isFloating(const char *str) {
  int i = 0;
  bool readPoint = false;
  while (str[i] != '\0') {
    if (str[i] == '.') {
      if (readPoint) {
        return false;
      } else {
        readPoint = true;
      }
    } else if (str[i] < '0' || str[i] > '9') {
      return false;
    }
    i++;
  }
  if (!readPoint || i == 0) {
    return false;
  }
  return true;
}

double *readFloating(const char *str, int start, int end) {
  double *res = malloc(sizeof(double));
  *res = 0.0;
  int i = start;
  while (str[i] != '.') {
    int n = (int)(str[i] - '0');
    *res *= 10.0;
    *res += (double)n;
    i++;
    // printf("float (start): read %d, thus res is %f\n", n, *res);
  }
  double rest = 0.0;
  for (int j = end - 1; j > i; j--) {
    int n = (int)(str[j] - '0');
    rest = 0.1 * rest + n;
    // printf("float (end): read %d, thus rest is %f\n", n, rest);
  }
  *res = *res + 0.1 * rest;
  return res;
}

void writeString(FILE *file, const char *str) {
  int n = stringLength(str);
  writeULEB128(file, n);
  for (int i = 0; i < n; i++) {
    writeByte(file, str[i]);
  }
}
