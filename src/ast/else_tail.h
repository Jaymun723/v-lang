#pragma once
#include "../lexer.h"
#include "expr.h"
#include <stdio.h>

// Forward declarations
typedef struct AstStatementList AstStatementList;
typedef struct AstStatement AstStatement;
typedef struct AstElseTail AstElseTail;

// AstStatement *parseIfStatement(TokenList *tkl);
// AstStatementList *parseBlock(TokenList *tkl);
// void freeAstStatement(AstStatement *statement);
// void freeAstStmtList(AstStatementList *stmtList);
// void fprintfAstStmtList(FILE *channel, AstStatementList *stmtList, int
// depth); void fprintfAstStatement(FILE *channel, AstStatement *statement, int
// depth);

// AstStatement ===
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
// === AstStatement

// AstStatementList
typedef struct AstStatementList {
  unsigned int numStatement;

  AstStatement *statementHead;
  AstStatement *statementTail;
} AstStatementList;

void addAstStatement(AstStatementList *stmtList, AstStatement *statement);
AstStatementList *parseStmtList(TokenList *tkl);
AstStatementList *parseBlock(TokenList *tkl);
void freeAstStmtList(AstStatementList *stmtList);
void fprintfAstStmtList(FILE *channel, AstStatementList *stmtList, int depth);
void printfAstStmtList(AstStatementList *stmtList);
// === AstStatementList

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