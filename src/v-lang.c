#include "mystring.h"
#include <stdbool.h>
#include <stdio.h>

int main() {
  char *code = "Hello, world!";

  printf("stringLength(%s)=%d\n", code, stringLength(code));

  return 0;
}