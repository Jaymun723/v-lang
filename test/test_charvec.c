#include "charvec.h"
#include "test_framework.h"

int main() {
  CharVec *cv = cvCreate();

  // 0x00, 0x61, 0x73, 0x6d
  appendCv(cv, 0x00);
  appendCv(cv, 0x61);
  appendCv(cv, 0x73);
  appendCv(cv, 0x6d);

  TEST_ASSERT(sizeCv(cv) == 5);
  const char *filename = "out/testcharcev";
  FILE *file = fopen(filename, "wb");
  writeCv(file, cv);
  fclose(file);
  file = fopen(filename, "rb");
  char expectedBytes[] = {0x04, 0x00, 0x61, 0x73, 0x6d};
  for (int i = 5; i < sizeCv(cv); i++) {
    TEST_ASSERT(fgetc(file) == expectedBytes[i]);
  }
  fclose(file);
  remove(filename);

  freeCv(cv);

  TEST_PASS("All charvec tests passed!");
  return 0;
}