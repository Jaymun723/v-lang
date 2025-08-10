#include "emitter.h"
#include "common.h"
#include "leb128.h"
#include "wasm/const.h"
#include <stdlib.h>

void funcMapperSet(FuncMapper *fm, const char *funcName, unsigned int index,
                   unsigned int typeIndex, bool import) {
  FuncDesc *desc = (FuncDesc *)malloc(sizeof(FuncDesc));
  desc->index = index;
  desc->typeIndex = typeIndex;
  desc->import = import;
  hashMapSet((HashMap *)fm, funcName, (void *)desc);
}

FuncDesc *funcMapperGet(FuncMapper *fm, const char *funcName) {
  return (FuncDesc *)hashMapGet((HashMap *)fm, funcName);
}

void freeFuncMapper(FuncMapper *fm) { freeHashMap((HashMap *)fm, free); }

void fprintfFuncDesc(FILE *channel, void *value) {
  FuncDesc *funcDesc = (FuncDesc *)value;
  fprintf(channel, "index: %d, typeIndex: %d, import: %d", funcDesc->index,
          funcDesc->typeIndex, funcDesc->import);
}
void printfFuncMapper(FuncMapper *fm) {
  fprintfHashMap(stdout, (HashMap *)fm, fprintfFuncDesc);
}

void emitByteToMainCode(WasmModule *module, WasmConst byte) {
  printf("emit: %s\n", WASM_CONST_STRING[byte]);
  appendCv(module->codeSection->codeHead->body, WASM_CONST_CODE[byte]);
}

void emitSLEB128ToMainCode(WasmModule *module, int value) {
  printf("emit: SLEB128(%d)\n", value);
  emitSLEB128(module->codeSection->codeHead->body, value);
}

void emitULEB128ToMainCode(WasmModule *module, unsigned int value) {
  printf("emit: ULEB128(%d)\n", value);
  emitULEB128(module->codeSection->codeHead->body, value);
}

void emitExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr);

void emitConstantExpr(WasmModule *module, AstExpr *expr) {
  if (expr->constant.type == TokenFloating) {
    fprintf(stderr, "flotting constants not implemented yet.\n");
  } else {
    emitByteToMainCode(module, WasmConst_i32_const);
    emitSLEB128ToMainCode(module, *(int *)expr->constant.value);
  }
}
void emitUnaryOpExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  emitExpr(module, fm, expr->unary.child);
  emitByteToMainCode(module, WasmConst_i32_neg);
}
void emitBinaryOpExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  emitExpr(module, fm, expr->binary.left);
  emitExpr(module, fm, expr->binary.right);
  switch (expr->binary.op) {
  case TokenAdd:
    emitByteToMainCode(module, WasmConst_i32_add);
    break;
  case TokenMinus:
    emitByteToMainCode(module, WasmConst_i32_sub);
    break;
  case TokenMult:
    emitByteToMainCode(module, WasmConst_i32_mul);
    break;
  case TokenDiv:
    emitByteToMainCode(module, WasmConst_i32_div_s);
    break;
  case TokenMod:
    emitByteToMainCode(module, WasmConst_i32_rem_s);
    break;

  default:
    break;
  }
}
void emitFuncCallExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  FuncDesc *funcDesc = funcMapperGet(fm, expr->funcCall.funcName);
  if (funcDesc == NULL) {
    fprintf(stderr, "emitFuncCallExpr: Unknow function called.\n");
    return;
  }
  emitExpr(module, fm, expr->funcCall.arg);
  emitByteToMainCode(module, WasmConst_call);
  emitULEB128ToMainCode(module, funcDesc->index);
}

void emitExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  switch (expr->type) {
  case AstExprConstant:
    emitConstantExpr(module, expr);
    break;
  case AstExprUnaryOp:
    emitUnaryOpExpr(module, fm, expr);
    break;
  case AstExprBinaryOp:
    emitBinaryOpExpr(module, fm, expr);
    break;
  case AstExprFuncCall:
    emitFuncCallExpr(module, fm, expr);
    break;
  default:
    break;
  }
}

void emitStatement(WasmModule *module, FuncMapper *fm,
                   AstStatement *statement) {
  if (statement->type == AstStmtFuncCall) {
    emitExpr(module, fm, statement->funcCall);
  }
}

void emit(WasmModule *module, AstProgram *program) {
  FuncMapper *fm = (FuncMapper *)createHashMap();
  funcMapperSet(fm, "print", 0, 1, true);

  for (AstStatement *statement = program->statementHead; statement != NULL;
       statement = statement->next) {
    emitStatement(module, fm, statement);
  }

  // printfFuncMapper(fm);
  emitByteToMainCode(module, WasmConst_end);
  freeFuncMapper(fm);
}