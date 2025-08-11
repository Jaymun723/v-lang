#pragma once
#include "../lexer.h"
#include "expr.h"
#include "statement_list.h"
#include <stdio.h>

typedef struct AstProgram {
  AstStatementList *stmtList;
} AstProgram;

AstProgram *parseAstProgram(TokenList *tkl);
void freeAstProgram(AstProgram *program);
void fprintfAstProgram(FILE *channel, AstProgram *program, int depth);
void printfAstProgram(AstProgram *program);