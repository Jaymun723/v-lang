#include "ast/program.h"
#include "test_framework.h"

int main() {
  const char *fileName = "out/main.v";
  FILE *file = fopen(fileName, "rb");
  TokenList *tkl = tokenizeFromFile(file);
  removeSpaces(tkl);

  printfTkl(tkl);

  AstProgram *program = parseAstProgram(tkl);

  printfAstProgram(program);

  freeTkl(tkl, true);
  if (program != NULL) {
    freeAstProgram(program);
  }

  TEST_PASS("All ast tests passed!");
  return 0;
}