#pragma once
#include "../lexer.h"
#include "expr.h"
#include "statement.h"
#include <stdio.h>

typedef struct AstProgram {
  unsigned int numStatements;

  AstStatement *statementHead;
  AstStatement *statementTail;
} AstProgram;

AstProgram *parseAstProgram(TokenList *tkl);
void freeAstProgram(AstProgram *program);
void fprintfAstProgram(FILE *channel, AstProgram *program);
void printfProgram(AstProgram *program);