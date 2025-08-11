#include "validation.h"
#include "wasm/const.h"

bool validateExprType(FuncMapper *fm, AstExpr *expr) {
  if (expr == NULL) {
    return true;
  } else if (expr->type == AstExprConstant) {
    return true;
  } else if (expr->type == AstExprFuncCall) {
    CharVec *results = getFunctionProperty(fm, expr->funcCall.funcName, 2);
    if ((results == NULL || results->length == 0)) {
      expr->evalType = AstEvalNone;
    } else if (results->length != 1) {
      fprintf(stderr,
              "validateExprType: Function %s has incorrect number of return "
              "types.\n",
              expr->funcCall.funcName);
      return false;
    } else {
      char resultType = getCv(results, 0);
      if (resultType == WASM_CONST_CODE[WasmConst_f64]) {
        expr->evalType = AstEvalFloating;
      } else if (resultType == WASM_CONST_CODE[WasmConst_i32]) {
        expr->evalType = AstEvalInteger;
      } else {
        fprintf(stderr,
                "validateExprType: Function %s has unknow return type.\n",
                expr->funcCall.funcName);
        return false;
      }
    }

    if (!validateExprType(fm, expr->funcCall.arg)) {
      return false;
    }

    CharVec *params = getFunctionProperty(fm, expr->funcCall.funcName, 0);
    if ((params == NULL || params->length == 0)) {
      if (expr->funcCall.arg != NULL) {
        fprintf(stderr,
                "validateExprType: Function %s does not take arguments.\n",
                expr->funcCall.funcName);
        return false;
      } else {
        return true;
      }
    } else if (params->length != 1) {
      fprintf(stderr, "not yet.\n");
      return false;
    } else {
      char paramType = getCv(params, 0);
      if (paramType == WASM_CONST_CODE[WasmConst_i32] &&
          expr->funcCall.arg->evalType == AstEvalInteger) {
        return true;
      } else if (paramType == WASM_CONST_CODE[WasmConst_f64] &&
                 expr->funcCall.arg->evalType == AstEvalFloating) {
        return true;
      } else {
        fprintf(stderr,
                "validateExprType: Function %s argument does not evaluate to "
                "function param type.\n",
                expr->funcCall.funcName);
        return false;
      }
    }
  } else if (expr->type == AstExprUnaryOp) {
    if (validateExprType(fm, expr->unary.child)) {
      expr->evalType = expr->unary.child->evalType;
      return true;
    } else {
      return false;
    }
  } else if (expr->type == AstExprBinaryOp) {
    if (validateExprType(fm, expr->binary.left) &&
        validateExprType(fm, expr->binary.right)) {
      if ((expr->binary.left->evalType == AstEvalFloating &&
           expr->binary.right->evalType == AstEvalFloating) ||
          (expr->binary.left->evalType == AstEvalInteger &&
           expr->binary.right->evalType == AstEvalInteger)) {
        expr->evalType = expr->binary.left->evalType;
        expr->binary.cast = AstExprBinaryNoCast;
        return true;
      } else if (expr->binary.left->evalType == AstEvalInteger &&
                 expr->binary.right->evalType == AstEvalFloating) {
        expr->evalType = AstEvalFloating;
        expr->binary.cast = AstExprBinaryCastLeft;
        return true;
      } else if (expr->binary.right->evalType == AstEvalInteger &&
                 expr->binary.left->evalType == AstEvalFloating) {
        expr->evalType = AstEvalFloating;
        expr->binary.cast = AstExprBinaryCastRight;
        return true;
      } else {
        fprintf(stderr,
                "validateExprType: Can decide with expression to cast.\n");
        return false;
      }
    } else {
      return false;
    }
  }
  fprintf(stderr, "validateExprType: Unknow expression type.\n");
  return false;
}

bool validateStmtList(FuncMapper *fm, AstStatementList *stmtList) {
  for (AstStatement *statement = stmtList->statementHead; statement != NULL;
       statement = statement->next) {
    if (!validateExprType(fm, statement->funcCall)) {
      return false;
    }
  }
  return true;
}

bool validate(FuncMapper *fm, AstProgram *program) {
  return validateStmtList(fm, program->stmtList);
}
