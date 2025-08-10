#include "validation.h"

bool validateExprType(AstExpr *expr) {
  if (expr->type == AstExprConstant) {
    return true;
  } else if (expr->type == AstExprFuncCall) {
  }
}

bool validate(FuncMapper *fm, AstProgram *program) { return true; }
