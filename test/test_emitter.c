#include "emitter.h"
#include "test_framework.h"
#include <stdio.h>

int main() {
  FILE *file = fopen("test.wasm", "wb");
  emit(file);
}