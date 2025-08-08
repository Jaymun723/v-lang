#include "leb128.h"
#include "test_framework.h"

int main() {
  const char *filename = "out/testleb128";
  FILE *file = fopen(filename, "wb");
  int n = 624485;
  writeULEB128(file, n);
  TEST_ASSERT(sizeULEB128(n) == 3);
  fclose(file);
  file = fopen(filename, "rb");
  char expectedUBytes[] = {(char)0xe5, (char)0x8e, 0x26};
  for (int i = 0; i < 3; i++) {
    TEST_ASSERT((char)fgetc(file) == expectedUBytes[i]);
  }
  fclose(file);

  file = fopen(filename, "wb");
  n = -123456;
  writeSLEB128(file, n);
  TEST_ASSERT(sizeULEB128(n) == 3);
  fclose(file);
  file = fopen(filename, "rb");
  char expectedSBytes[] = {(char)0xc0, (char)0xbb, 0x78};
  for (int i = 0; i < 3; i++) {
    TEST_ASSERT((char)fgetc(file) == expectedSBytes[i]);
  }
  fclose(file);

  remove(filename);
  TEST_PASS("All leb128 tests passed!");
  return 0;
}