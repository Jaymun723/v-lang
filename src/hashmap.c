#include "hashmap.h"
#include "mystring.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
static uint64_t hash_key(const char *key) {
  uint64_t hash = FNV_OFFSET;
  for (const char *p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

typedef struct HashMapEntry {
  const char *key;
  void *value;
} HashMapEntry;

typedef struct HashMap {
  size_t length;
  size_t capacity;

  HashMapEntry *entries;
} HashMap;

HashMap *createHashMap() {
  HashMap *hm = (HashMap *)malloc(sizeof(HashMap));
  hm->length = 0;
  hm->capacity = 5;
  hm->entries = (HashMapEntry *)malloc(sizeof(HashMapEntry) * hm->capacity);
  for (size_t i = 0; i < hm->capacity; i++) {
    hm->entries[i].key = NULL;
    hm->entries[i].value = NULL;
  }
  return hm;
}

unsigned int hashMapLength(HashMap *hm) { return (unsigned int)hm->length; }

bool hashMapSetEntry(HashMap *hm, const char *key, void *value) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(hm->capacity - 1));

  while (hm->entries[index].key != NULL) {
    if (stringCmp(hm->entries[index].key, key) == 0) {
      hm->entries[index].value = value;
      return true;
    }

    index++;
    if (index >= hm->capacity) {
      index = 0;
    }
  }

  hm->entries[index].key = key;
  hm->entries[index].value = value;

  return true;
}

bool hashMapExpand(HashMap *hm) {
  HashMapEntry *oldEntries = hm->entries;
  size_t oldCapacity = hm->capacity;

  hm->capacity = 2 * oldCapacity;
  hm->entries = malloc(sizeof(HashMapEntry) * hm->capacity);
  for (size_t i = 0; i < hm->capacity; i++) {
    hm->entries[i].key = NULL;
    hm->entries[i].value = NULL;
  }

  for (size_t i = 0; i < oldCapacity; i++) {
    hashMapSetEntry(hm, oldEntries[i].key, oldEntries[i].value);
  }

  free(oldEntries);

  return true;
}

bool hashMapSet(HashMap *hm, const char *key, void *value) {
  if (hm->length == hm->capacity) {
    if (!hashMapExpand(hm)) {
      return false;
    }
  }

  if (!hashMapSetEntry(hm, key, value)) {
    return false;
  }
  hm->length++;
  return true;
}

void *hashMapGet(HashMap *hm, const char *key) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(hm->capacity - 1));

  while (hm->entries[index].key != NULL) {
    if (stringCmp(hm->entries[index].key, key) == 0) {
      return hm->entries[index].value;
    }
    index++;
    if (index >= hm->capacity) {
      index = 0;
    }
  }
  return NULL;
}

void fprintfHashMapEntry(FILE *channel, HashMapEntry *hme) {
  if (hme->key != NULL) {
    fprintf(channel, "  %s -> %p\n", hme->key, hme->value);
  }
}

void fprintfHashMap(FILE *channel, HashMap *hm) {
  fprintf(channel, "HashMap(length=%ld, capacity=%ld\n", hm->length,
          hm->capacity);
  for (size_t i = 0; i < hm->capacity; i++) {
    fprintfHashMapEntry(channel, &hm->entries[i]);
  }
  fprintf(channel, ")\n");
}

void printfHashMap(HashMap *hm) { fprintfHashMap(stdout, hm); }

void freeHashMap(HashMap *hm) {
  free(hm->entries);
  free(hm);
}