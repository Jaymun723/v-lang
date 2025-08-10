#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct HashMap HashMap;

HashMap *createHashMap();
unsigned int hashMapLength(HashMap *hm);
bool hashMapSet(HashMap *hm, const char *key, void *value);
void *hashMapGet(HashMap *hm, const char *key);
void fprintfHashMap(FILE *channel, HashMap *hm,
                    void (*fprintfValue)(FILE *, void *));
void printfHashMap(HashMap *hm);
void freeHashMap(HashMap *hm, void (*freeValue)(void *));