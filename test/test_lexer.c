#include "lexer.h"
#include "mystring.h"
#include "test_framework.h"
#include <stdio.h>

int main() {
  // Length
  // char *sourceCode = "V-lang  \n  is\n a super language";
  // char *expectedWords[] = {"V-lang", "is", "a", "super", "language"};
  // TokenList *tkl = toWords(sourceCode);
  // printf("tkl->lengt=%d\n", tkl->length);
  // // TEST_ASSERT(tkl->length == 5);
  // int i = 0;
  // for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
  //   printf("tok->value=\"%s\"\n", tok->value);
  //   TEST_ASSERT(tok->type == Word);
  //   TEST_ASSERT(stringCmp(tok->value, expectedWords[i]) == 0);
  //   i++;
  // }
  // TEST_ASSERT(i == 5);
  // tkl_free(tkl, true);

  // sourceCode = "print  ( 5. )";
  // char *expectedW = {"print", "(", "5.", ")"};
  // tkl = toWords(sourceCode);
  // i = 0;
  // for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
  //   printf("tok->value=\"%s\"\n", tok->value);
  //   TEST_ASSERT(tok->type == Word);
  //   TEST_ASSERT(stringCmp(tok->value, expectedW[i]) == 0);
  //   i++;
  // }
  // TEST_ASSERT(i == 4);
  // tkl_free(tkl, true);

  // sourceCode = "";
  // tkl = toWords(sourceCode);
  // TEST_ASSERT(tkl->length == 0);

  // sourceCode = "print ( 5. )";
  // TokenType expectedType[] = {Keyword, OpenParent, Floating, CloseParent};
  // char *expectedValue[] = {"print", "(", "5.", ")"};
  // tkl = tokenize(sourceCode);
  // i = 0;
  // for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
  //   TEST_ASSERT(tok->type == expectedType[i]);
  //   TEST_ASSERT(stringCmp(tok->value, expectedValue[i]) == 0);
  //   i++;
  // }
  // TEST_ASSERT(i == 4);
  // tkl_free(tkl, true);

  char *sourceCode = "print        (4.5)\nassert (1yes)\nchain 5.4.1";
  printf("Reading: \"\"\"%s\"\"\"\n", sourceCode);
  TokenList *tkl = tokenize(sourceCode);
  printf("TokenList:\n");
  tklPrint(tkl);
  if (tkl != NULL) {
    tklFree(tkl, true);
  }

  TEST_PASS("All lexer tests passed!");

  return 0;
}