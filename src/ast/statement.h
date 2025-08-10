#pragma once
#include "../lexer.h"
#include "expr.h"
#include <stdio.h>

typedef enum AstStmtType { AstStmtFuncCall } AstStmtType;

typedef struct AstStatement {
  AstStmtType type;

  union {
    AstExpr *funcCall;
  };

  struct AstStatement *next;
} AstStatement;

AstStatement *parseAstStatement(TokenList *tkl);
void freeAstStatement(AstStatement *statement);
void fprintfAstStatement(FILE *channel, AstStatement *statement, int depth);