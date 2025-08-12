#pragma once
#include "../lexer.h"
#include "expr.h"
#include <stdio.h>

// Forward declarations
typedef struct AstStatementList AstStatementList;
typedef struct AstStatement AstStatement;
AstStatement *parseIfStatement(TokenList *tkl);
AstStatementList *parseBlock(TokenList *tkl);
void freeAstStatement(AstStatement *statement);
void freeAstStmtList(AstStatementList *stmtList);
void fprintfAstStmtList(FILE *channel, AstStatementList *stmtList, int depth);
void fprintfAstStatement(FILE *channel, AstStatement *statement, int depth);

typedef enum AstElseTailType {
  AstElseTailEmpty,
  AstElseTailElse,
  AstElseTailElif,
} AstElseTailType;

typedef struct AstElseTail {
  AstElseTailType type;

  union {
    AstStatementList *block;
    AstStatement *elif;
  };
} AstElseTail;

AstElseTail *parseAstElseTail(TokenList *tkl);
void freeAstElseTail(AstElseTail *tail);
void fprintfAstElseTail(FILE *channel, AstElseTail *tail, int depth);