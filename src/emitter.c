#include "emitter.h"
#include "common.h"
#include "func_mapper.h"
#include "leb128.h"
#include "wasm/const.h"
#include <stdlib.h>

void emitByteToMainCode(WasmModule *module, WasmConst byte) {
  appendCv(module->codeSection->codeHead->body, WASM_CONST_CODE[byte]);
}

void emitSLEB128ToMainCode(WasmModule *module, int value) {
  emitSLEB128(module->codeSection->codeHead->body, value);
}

void emitULEB128ToMainCode(WasmModule *module, unsigned int value) {
  emitULEB128(module->codeSection->codeHead->body, value);
}

void emitIEEE754ToMainCode(WasmModule *module, double value) {
  unsigned char *bytes = (unsigned char *)&value;
  for (int i = 0; i < 8; i++) {
    emitByteToMainCode(module, bytes[i]);
  }
}

void emitExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr);

void emitConstantExpr(WasmModule *module, AstExpr *expr) {
  if (expr->constant.type == TokenFloating) {
    emitByteToMainCode(module, WasmConst_f64_const);
    emitIEEE754ToMainCode(module, *(double *)expr->constant.value);
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
  int i = getFunctionIndex(fm, expr->funcCall.funcName);
  if (i == -1) {
    fprintf(stderr, "emitFuncCallExpr: Unknow function called.\n");
    return;
  }
  emitExpr(module, fm, expr->funcCall.arg);
  emitByteToMainCode(module, WasmConst_call);
  emitULEB128ToMainCode(module, (unsigned int)i);
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
  FuncMapper *fm = createFuncMapper();

  CharVec *printI32Params = cvCreate();
  appendCv(printI32Params, WASM_CONST_CODE[WasmConst_i32]);
  addFunction(fm, "print_i32", printI32Params, NULL, NULL, true);

  CharVec *printF64Params = cvCreate();
  appendCv(printF64Params, WASM_CONST_CODE[WasmConst_f64]);
  addFunction(fm, "print_f64", printF64Params, NULL, NULL, true);

  unsigned int startIndex = addFunction(fm, "start", NULL, NULL, NULL, false);
  module->startSection->index = startIndex + 2;

  for (AstStatement *statement = program->statementHead; statement != NULL;
       statement = statement->next) {
    emitStatement(module, fm, statement);
  }
  emitByteToMainCode(module, WasmConst_end);

  emitFuncMapper(module, fm);
  freeFuncMapper(fm);
}