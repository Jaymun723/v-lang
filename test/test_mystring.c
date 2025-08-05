#include "mystring.h"
#include "test_framework.h"
#include <stdio.h>

int main() {
  // Length
  int n_strings = 3;
  char *strings[] = {"aaa", "", "4"};
  int lengths[] = {3, 0, 1};
  for (int i = 0; i < n_strings; i++) {
    // printf("stringLength(\"%s\")==%d\n", strings[i], lengths[i]);
    TEST_ASSERT(stringLength(strings[i]) == lengths[i]);
  }

  // Compare
  n_strings = 6;
  char *a_strings[] = {"aaa", "", "", "a", "b", "4", "aa", "a"};
  char *b_strings[] = {"aaa", "", "a", "", "a", "5", "a", "aa"};
  int cmp[] = {0, 0, -1, 1, 1, -1, 1, -1};
  for (int i = 0; i < n_strings; i++) {
    // printf("(stringCmp(\"%s\", \"%s\")=%d)==%d\n", a_strings[i],
    // b_strings[i],
    //        stringCmp(a_strings[i], b_strings[i]), cmp[i]);
    TEST_ASSERT(stringCmp(a_strings[i], b_strings[i]) == cmp[i]);
  }

  // CopyN
  n_strings = 3;
  char *toCopyStrings[] = {"aaa", "aaa", "aaa"};
  char *targetStrings[] = {"aaa", "aa", ""};
  int copyN[] = {-1, 2, 0};
  for (int i = 0; i < n_strings; i++) {
    char *copy = stringCopyN(toCopyStrings[i], copyN[i]);
    TEST_ASSERT(stringCmp(copy, targetStrings[i]) == 0);
  }

  // isInteger && isFloatting
  n_strings = 8;
  char *numberStrings[] = {"04", "a", "0xf", "0.1", "180", "", ".8", "4."};
  bool areIntegers[] = {true, false, false, false, true, false, false, false};
  bool areFloatting[] = {false, false, false, true, false, false, true, true};
  for (int i = 0; i < n_strings; i++) {
    TEST_ASSERT(isInteger(numberStrings[i]) == areIntegers[i]);
    // printf("(isFloating(\"%s\")=%d)==%d\n", numberStrings[i],
    //        isFloating(numberStrings[i]), areFloatting[i]);
    TEST_ASSERT(isFloating(numberStrings[i]) == areFloatting[i]);
  }

  TEST_PASS("All mystring tests passed!");
  return 0;
}