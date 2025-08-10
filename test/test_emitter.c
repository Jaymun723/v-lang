#include "emitter.h"
#include "lexer.h"
#include "test_framework.h"

int main() {
  const char *sourceCode = "print(4-8);\nprint((1+5)/2);";
  TokenList *tkl = tokenize(sourceCode);
  removeSpaces(tkl);
  printfTkl(tkl);
  WasmModule *module = createWasmModule();
  printfWasmModule(module);
  AstProgram *program = parseAstProgram(tkl);
  printfAstProgram(program);

  emit(module, program);

  printfWasmModule(module);

  const char *fileName = "out/main.wasm";
  FILE *file = fopen(fileName, "wb");

  writeModule(file, module);

  fclose(file);
  freeAstProgram(program);
  freeWasmModule(module);
  freeTkl(tkl, true);

  TEST_PASS("All emitter tests passed!");
  return 0;
}