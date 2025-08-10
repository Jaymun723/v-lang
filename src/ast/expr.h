#pragma once
#include "../lexer.h"
#include <stdio.h>

#define GENERATE_AST_EXPR_TYPE_ENUM(ENUM) AstExpr##ENUM,
#define GENERATE_AST_EXPR_TYPE_STRING(STRING) #STRING,

#define FOREACH_AST_EXPR_TYPE(AST_EXPR_TYPE)                                   \
  AST_EXPR_TYPE(Constant)                                                      \
  AST_EXPR_TYPE(UnaryOp)                                                       \
  AST_EXPR_TYPE(BinaryOp)                                                      \
  AST_EXPR_TYPE(FuncCall)

typedef enum AstExprType {
  FOREACH_AST_EXPR_TYPE(GENERATE_AST_EXPR_TYPE_ENUM) AstExprTypeCount
} AstExprType;

extern const char *AST_EXPR_TYPE_STRING[];

typedef struct AstExpr {
  AstExprType type;
  union {
    struct {
      TokenType type;
      void *value;
    } constant;
    struct {
      TokenType op;
      struct AstExpr *child;
    } unary;
    struct {
      TokenType op;
      struct AstExpr *left;
      struct AstExpr *right;
    } binary;
    struct {
      char *funcName;
      struct AstExpr *arg;
    } funcCall;
  };
} AstExpr;

AstExpr *parseExpr(TokenList *tkl);
AstExpr *parseExprPrecedence(TokenList *tkl, int minPrec);
AstExpr *parseUnaryExpr(TokenList *tkl);

void freeAstExpr(AstExpr *expr);
void fprintfAstExpr(FILE *channel, AstExpr *expr, int depth);