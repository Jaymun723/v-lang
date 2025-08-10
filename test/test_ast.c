#include "ast/program.h"
#include "test_framework.h"

int main() {
  const char *sourceCode = "print(4 - 7 * 2);\nassert(7+8+2+-.2);";
  TokenList *tkl = tokenize(sourceCode);
  removeSpaces(tkl);

  printfTkl(tkl);

  AstProgram *program = parseAstProgram(tkl);

  printfProgram(program);

  freeTkl(tkl, true);
  if (program != NULL) {
    freeAstProgram(program);
  }

  TEST_PASS("All ast tests passed!");
  return 0;
}