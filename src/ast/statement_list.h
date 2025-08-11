#pragma once
#include "lexer.h"
#include "statement.h"

typedef struct AstStatementList {
  unsigned int numStatement;

  AstStatement *statementHead;
  AstStatement *statementTail;
} AstStatementList;

void addAstStatement(AstStatementList *stmtList, AstStatement *statement);
AstStatementList *parseStmtList(TokenList *tkl);
void freeAstStmtList(AstStatementList *stmtList);
void fprintfAstStmtList(FILE *channel, AstStatementList *stmtList, int depth);
void printfAstStmtList(AstStatementList *stmtList);