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
    // printf("copy=%s, targetString[%d]=%s\n", copy, i, targetStrings[i]);
    TEST_ASSERT(stringCmp(copy, targetStrings[i]) == 0);
    free(copy);
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

  // readIntegers
  n_strings = 4;
  char *strInts[] = {"000", "08", "7503", "30"};
  int intsStarts[] = {0, 0, 1, 0};
  int intsEnds[] = {3, 2, 4, 1};
  int matchInts[] = {0, 8, 503, 3};
  for (int i = 0; i < n_strings; i++) {
    int *v = readInteger(strInts[i], intsStarts[i], intsEnds[i]);
    TEST_ASSERT(*v == matchInts[i]);
    free(v);
  }

  // readFloating
  n_strings = 5;
  char *strFloats[] = {"000.0", "0.8", "75.03", ".30", "12.34", "4."};
  int floatsStart[] = {0, 0, 1, 0, 1, 0};
  int floatsEnd[] = {4, 3, 5, 2, 4, 2};
  double matchFloats[] = {0.0, 0.8, 5.03, 0.3, 2.3, 4.0};
  for (int i = 0; i < n_strings; i++) {
    double *v = readFloating(strFloats[i], floatsStart[i], floatsEnd[i]);

    double tol = 0.0000000000000001;
    double diff = *v - matchFloats[i];
    TEST_ASSERT(diff > -tol);
    TEST_ASSERT(diff < tol);
    free(v);
  }

  TEST_PASS("All mystring tests passed!");
  return 0;
}