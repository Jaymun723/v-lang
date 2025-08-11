#include "expr.h"
#include "../mystring.h"
#include <stdbool.h>
#include <stdlib.h>

const char *AST_EXPR_TYPE_STRING[] = {
    FOREACH_AST_EXPR_TYPE(GENERATE_AST_EXPR_TYPE_STRING)};

const char *AST_EVAL_TYPE_STRING[] = {
    FOREACH_AST_EVAL_TYPE(GENERATE_AST_EVAL_TYPE_STRING)};

int getPrecedence(TokenType type) {
  switch (type) {
  case TokenMult:
  case TokenDiv:
  case TokenMod:
    return 3;
  case TokenAdd:
  case TokenMinus:
    return 2;
  default:
    return 0;
  }
}

AstExpr *createConstantExpr(Token *tok) {
  AstExpr *expr = (AstExpr *)malloc(sizeof(AstExpr));
  expr->type = AstExprConstant;
  if (tok->type == TokenInteger) {
    expr->evalType = AstEvalInteger;
    expr->constant.value = (void *)malloc(sizeof(int));
    *((int *)expr->constant.value) = *((int *)tok->value);
  } else {
    expr->evalType = AstEvalFloating;
    expr->constant.value = (void *)malloc(sizeof(double));
    *((double *)expr->constant.value) = *((double *)tok->value);
  }
  return expr;
}

AstExpr *createUnaryExpr(AstExpr *child) {
  AstExpr *expr = (AstExpr *)malloc(sizeof(AstExpr));
  expr->evalType = AstEvalUndefined;
  expr->type = AstExprUnaryOp;
  expr->unary.op = TokenMinus;
  expr->unary.child = child;
  return expr;
}

AstExpr *createFuncCallExpr(char *funcName, AstExpr *arg) {
  AstExpr *expr = (AstExpr *)malloc(sizeof(AstExpr));
  expr->evalType = AstEvalUndefined;
  expr->type = AstExprFuncCall;
  expr->funcCall.funcName = funcName;
  expr->funcCall.arg = arg;
  return expr;
}

AstExpr *createBinaryExpr(TokenType opType, AstExpr *left, AstExpr *right) {
  AstExpr *expr = (AstExpr *)malloc(sizeof(AstExpr));
  expr->evalType = AstEvalUndefined;
  printf("expr->evalType = AstEvalUndefined = %d;", AstEvalUndefined);
  expr->type = AstExprBinaryOp;
  expr->binary.op = opType;
  expr->binary.left = left;
  expr->binary.right = right;
  expr->binary.cast = AstExprBinaryNoCast;
  return expr;
}

AstExpr *parseExpr(TokenList *tkl) { return parseExprPrecedence(tkl, 1); }

AstExpr *parseUnaryExpr(TokenList *tkl) {
  Token *tok = tklPeek(tkl);
  if (!tok) {
    return NULL;
  }

  if (tok->type == TokenOpenParent) {
    tklPop(tkl);
    AstExpr *expr = parseExpr(tkl);
    tok = tklPeek(tkl);
    if (tok->type != TokenCloseParent) {
      fprintf(stderr, "parseUnaryExpr: Expected closing parenthesis.\n");
      freeAstExpr(expr);
      return NULL;
    }
    tklPop(tkl);
    return expr;
  }

  if (tok->type == TokenInteger || tok->type == TokenFloating) {
    AstExpr *expr = createConstantExpr(tok);
    tklPop(tkl);
    return expr;
  }

  if (tok->type == TokenMinus) {
    tklPop(tkl);
    AstExpr *child = parseUnaryExpr(tkl);
    if (!child) {
      return NULL;
    }
    return createUnaryExpr(child);
  }

  if (tok->type == TokenIdentifier) {
    char *funcName = stringCopyN(tok->value, -1);
    tklPop(tkl);

    Token *next = tklPeek(tkl);
    if (next && next->type == TokenOpenParent) {
      tklPop(tkl);
      AstExpr *arg = parseExpr(tkl);
      tok = tklPeek(tkl);
      if (tok->type != TokenCloseParent) {
        fprintf(stderr,
                "parseUnaryExpr: Expected closing parenthesis in func call.\n");
        free(funcName);
        freeAstExpr(arg);
        return NULL;
      }
      tklPop(tkl);
      return createFuncCallExpr(funcName, arg);
    } else {
      free(funcName);
      fprintf(stderr, "parseUnaryExpr: Unexpected token after identifier; "
                      "expected '(' for func call.\n");
      return NULL;
    }
  }

  fprintf(stderr, "parseUnaryExpr: Unexpected token in .\n");
  return NULL;
}

AstExpr *parseExprPrecedence(TokenList *tkl, int minPrec) {
  AstExpr *left = parseUnaryExpr(tkl);
  if (!left) {
    return NULL;
  }

  while (true) {
    Token *operator = tklPeek(tkl);
    if (!operator) {
      break;
    }

    int prec = getPrecedence(operator->type);
    if (prec < minPrec) {
      break;
    }

    TokenType opType = operator->type;
    tklPop(tkl);

    AstExpr *right = parseExprPrecedence(tkl, prec + 1);
    if (!right) {
      freeAstExpr(left);
      return NULL;
    }

    AstExpr *binExpr = createBinaryExpr(opType, left, right);
    left = binExpr;
  }
  return left;
}

void freeAstExpr(AstExpr *expr) {
  if (expr->type == AstExprConstant) {
    free(expr->constant.value);
  } else if (expr->type == AstExprUnaryOp) {
    freeAstExpr(expr->unary.child);
  } else if (expr->type == AstExprBinaryOp) {
    freeAstExpr(expr->binary.left);
    freeAstExpr(expr->binary.right);
  } else if (expr->type == AstExprFuncCall) {
    freeAstExpr(expr->funcCall.arg);
    free(expr->funcCall.funcName);
  }
  free(expr);
}

void fprintfAstExpr(FILE *channel, AstExpr *expr, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  if (AST_EXPR_TYPE_STRING[expr->type] == NULL) {
    fprintf(channel, "NULL\n");
    return;
  }
  if (expr->type == AstExprConstant) {
    if (expr->evalType == AstEvalInteger) {
      fprintf(channel, "%d)\n", *(int *)expr->constant.value);
    } else {
      fprintf(channel, "%f)\n", *(double *)expr->constant.value);
    }
  } else if (expr->type == AstExprUnaryOp) {
    fprintf(channel, "%s,\n", TOKEN_TYPE_STRING[expr->unary.op]);
    fprintfAstExpr(channel, expr->unary.child, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, ")\n");
  } else if (expr->type == AstExprBinaryOp) {
    fprintf(channel, "%s,\n", TOKEN_TYPE_STRING[expr->unary.op]);
    fprintfAstExpr(channel, expr->binary.left, depth + 1);
    fprintfAstExpr(channel, expr->binary.right, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, ")\n");
  } else if (expr->type == AstExprFuncCall) {
    fprintf(channel, "%s,\n", expr->funcCall.funcName);
    fprintfAstExpr(channel, expr->funcCall.arg, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, ")\n");
  }
}
