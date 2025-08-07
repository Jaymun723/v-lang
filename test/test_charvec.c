#include "charvec.h"
#include "test_framework.h"

int main() {
  CharVec *cv = cvCreate();

  // 0x00, 0x61, 0x73, 0x6d
  appendCv(cv, 0x00);
  appendCv(cv, 0x61);
  appendCv(cv, 0x73);
  appendCv(cv, 0x6d);

  cvPrint(cv);
  freeCv(cv);

  TEST_PASS("All charvec tests passed!");
  return 0;
}