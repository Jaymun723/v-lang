#pragma once
#include "../lexer.h"
#include "expr.h"
#include <stdio.h>

typedef struct AstStmtType {
  AstStmtFuncCall
} AstStmtType;

typedef struct AstStatement {
  AstStmtType type;

  union {
    AstExpr *funcCall;
  };

  struct AstStatement *next;
} AstStatement;

AstStatement *parseAstStatement(TokenList *tkl);
void freeAstStatement(AstStatement *statement);
void fprintfAstStatement(FILE *channel, AstStatement *statement);