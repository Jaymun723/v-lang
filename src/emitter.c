#include "emitter.h"
#include "common.h"
#include "func_mapper.h"
#include "leb128.h"
#include "wasm/const.h"
#include <stdlib.h>

void emitByteToMainCode(WasmModule *module, WasmConst byte) {
  appendCv(module->codeSection->codeHead->body, WASM_CONST_CODE[byte]);
}

void emitRawByteToMainCode(WasmModule *module, char byte) {
  appendCv(module->codeSection->codeHead->body, byte);
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
    emitRawByteToMainCode(module, bytes[i]);
  }
}

void emitExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr);

void emitConstantExpr(WasmModule *module, AstExpr *expr) {
  if (expr->evalType == AstEvalFloating) {
    emitByteToMainCode(module, WasmConst_f64_const);
    emitIEEE754ToMainCode(module, *(double *)expr->constant.value);
  } else {
    emitByteToMainCode(module, WasmConst_i32_const);
    emitSLEB128ToMainCode(module, *(int *)expr->constant.value);
  }
}
void emitUnaryOpExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  emitExpr(module, fm, expr->unary.child);
  if (expr->evalType == AstEvalInteger) {
    emitByteToMainCode(module, WasmConst_i32_neg);
  } else {
    emitByteToMainCode(module, WasmConst_f64_neg);
  }
}
void emitBinaryOpExpr(WasmModule *module, FuncMapper *fm, AstExpr *expr) {
  emitExpr(module, fm, expr->binary.left);
  if (expr->binary.cast == AstExprBinaryCastLeft) {
    emitByteToMainCode(module, WasmConst_f64_convert_i32_s);
  }
  emitExpr(module, fm, expr->binary.right);
  if (expr->binary.cast == AstExprBinaryCastRight) {
    emitByteToMainCode(module, WasmConst_f64_convert_i32_s);
  }

  bool isInteger = expr->evalType == AstEvalInteger;

  switch (expr->binary.op) {
  case TokenAdd:
    emitByteToMainCode(module,
                       isInteger ? WasmConst_i32_add : WasmConst_f64_add);
    break;
  case TokenMinus:
    emitByteToMainCode(module,
                       isInteger ? WasmConst_i32_sub : WasmConst_f64_sub);
    break;
  case TokenMult:
    emitByteToMainCode(module,
                       isInteger ? WasmConst_i32_mul : WasmConst_f64_mul);
    break;
  case TokenDiv:
    emitByteToMainCode(module,
                       isInteger ? WasmConst_i32_div_s : WasmConst_f64_div);
    break;
  case TokenMod:
    fprintf(stderr, "should be validated but pagnan you konw.\n");
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

void emitStmtList(WasmModule *module, FuncMapper *fm,
                  AstStatementList *stmtList) {
  for (AstStatement *statement = stmtList->statementHead; statement != NULL;
       statement = statement->next) {
    emitStatement(module, fm, statement);
  }
}

void emit(WasmModule *module, FuncMapper *fm, AstProgram *program) {
  module->startSection->index = getFunctionIndex(fm, "start");

  emitStmtList(module, fm, program->stmtList);
  emitByteToMainCode(module, WasmConst_end);

  emitFuncMapper(module, fm);
}