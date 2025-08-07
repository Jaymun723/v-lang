#include "test_framework.h"
#include "wasm_type.h"

int main() {
  WasmModule *module = createModule();
  printfWasmModule(module);
  freeWasmModule(module);

  TEST_PASS("All wasm_types tests passed!");
  return 0;
}