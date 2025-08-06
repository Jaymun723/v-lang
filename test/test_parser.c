#include "lexer.h"
#include "parser.h"
#include "test_framework.h"
#include <stdio.h>

int main() {
  char *sourceCode = "print(4.5)";
  printf("Reading: \"\"\"%s\"\"\"\n", sourceCode);
  TokenList *tkl = tokenize(sourceCode);
  printf("TokenList:\n");
  tklPrint(tkl);
  AstNode *node = read_statement(tkl, true);
  printf("Intermediate representation:\n");
  astPrintroot(node);
  astFree(node);
  tklFree(tkl, true);

  TEST_PASS("All parser tests passed!");
  return 0;
}