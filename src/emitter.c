#include "emitter.h"
#include "common.h"
#include "wasm/const.h"

void emitStatement(FILE *file, AstStatement *statement, WasmModule *module) {}

void emit(FILE *file, AstProgram *program, WasmModule *module) {
  writeModule(file, module);

  for (AstStatement *statement = program->statementHead; statement != NULL;
       statement = statement->next) {
    emitStatement(file, statement, module);
  }

  appendCv(module->codeSection->codeHead->body, WasmConst_end);
}