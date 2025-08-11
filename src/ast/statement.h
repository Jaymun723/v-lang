#pragma once
#include "../lexer.h"
#include "else_tail.h"
#include "expr.h"
#include "statement_list.h"
#include <stdio.h>

#define GENERATE_AST_STMT_TYPE_ENUM(ENUM) AstStmt##ENUM,
#define GENERATE_AST_STMT_TYPE_STRING(STRING) #STRING,

#define FOREACH_AST_STMT_TYPE(AST_STMT_TYPE)                                   \
  AST_STMT_TYPE(While)                                                         \
  AST_STMT_TYPE(If)                                                            \
  AST_STMT_TYPE(FuncCall)

typedef enum AstStmtType {
  FOREACH_AST_STMT_TYPE(GENERATE_AST_STMT_TYPE_ENUM) AstStmtTypeCount
} AstStmtType;

extern const char *AST_STMT_TYPE_STRING[];

typedef struct AstStatement {
  AstStmtType type;

  union {
    AstExpr *funcCall;
    struct {
      AstExpr *condition;
      AstStatementList *block;
    } While;
    struct {
      AstExpr *condition;
      AstStatementList *block;
      AstElseTail *tail;
    } If;
  };

  struct AstStatement *next;
} AstStatement;

AstStatement *parseIfStatement(TokenList *tkl);
AstStatement *parseAstStatement(TokenList *tkl);
void freeAstStatement(AstStatement *statement);
void fprintfAstStatement(FILE *channel, AstStatement *statement, int depth);