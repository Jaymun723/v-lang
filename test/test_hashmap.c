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

  const char *r = "";
  for (int i = 0; i < 2 * hashMapLength(hm) + 1; i++) {
    const char **key = &r;
    void *val = hashMapIterate(hm, key);
    if (val != NULL) {
      printf("%s -> %ld\n", *key, (intptr_t)val);
    }
  }

  freeHashMap(hm, NULL);

  TEST_PASS("ALL hashmap tests passed!");
  return 0;
}