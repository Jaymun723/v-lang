#include "mystring.h"
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
  } else if (n == 0) {
    return NULL;
  }
  char *res = (char *)malloc(sizeof(char) * (n + 1));
  for (int i = 0; i < n; i++) {
    res[i] = source[i];
  }
  res[n] = '\0';
  return res;
}

bool isNum(char c) { return c >= '0' && c <= '9'; }

bool isAlpha(char c) {
  if (c >= 'a' && c <= 'z')
    return true;
  if (c >= 'A' && c <= 'Z')
    return true;
  return false;
}

bool isAlphanum(char c) { return isNum(c) || isAlpha(c); }

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
