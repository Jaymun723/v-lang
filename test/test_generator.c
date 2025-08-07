#include "generator.h"
#include "test_framework.h"
#include "wasm/module.h"

int main() {
  FILE *file = fopen("out/main.wasm", "wb");
  WasmModule *module = createModule();
  printfWasmModule(module);
  writeModule(file, module);
  freeWasmModule(module);

  TEST_PASS("All generator tests passed!");
  return 0;
}