#include "hashmap.h"
#include "test_framework.h"
#include <stdint.h>

void fprintfInt(FILE *channel, void *value) {
  fprintf(channel, "%ld", (intptr_t)value);
}

int main() {
  HashMap *hm = createHashMap();

  hashMapSet(hm, "toto", (void *)((intptr_t)1));
  hashMapSet(hm, "tata", (void *)((intptr_t)2));

  hashMapSet(hm, "teuteu", (void *)((intptr_t)3));
  hashMapSet(hm, "coco", (void *)((intptr_t)4));
  hashMapSet(hm, "cute", (void *)((intptr_t)5));
  hashMapSet(hm, "olpoe", (void *)((intptr_t)6));
  fprintfHashMap(stdout, hm, fprintfInt);

  freeHashMap(hm, NULL);

  TEST_PASS("ALL hashmap tests passed!");
  return 0;
}