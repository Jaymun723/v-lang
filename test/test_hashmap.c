#include "hashmap.h"
#include "test_framework.h"
#include <stdint.h>

int main() {
  HashMap *hm = createHashMap();

  hashMapSet(hm, "toto", (void *)((intptr_t)1));
  hashMapSet(hm, "tata", (void *)((intptr_t)2));

  hashMapSet(hm, "teuteu", (void *)((intptr_t)3));
  hashMapSet(hm, "coco", (void *)((intptr_t)4));
  hashMapSet(hm, "cute", (void *)((intptr_t)5));
  hashMapSet(hm, "olpoe", (void *)((intptr_t)6));
  printfHashMap(hm);

  freeHashMap(hm);

  TEST_PASS("ALL hashmap tests passed!");
  return 0;
}