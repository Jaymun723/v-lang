#include "program.h"
#include <stdio.h>
#include <stdlib.h>

AstProgram *createProgram() {
  AstProgram *program = (AstProgram *)malloc(sizeof(AstProgram));
  program->stmtList = NULL;
  return program;
}

AstProgram *parseAstProgram(TokenList *tkl) {
  AstStatementList *stmtList = parseStmtList(tkl);
  if (stmtList == NULL) {
    return NULL;
  }
  AstProgram *program = createProgram();
  program->stmtList = stmtList;
  return program;
}

void freeAstProgram(AstProgram *program) {
  freeAstStmtList(program->stmtList);
  free(program);
}

void fprintfAstProgram(FILE *channel, AstProgram *program, int depth) {
  fprintfAstStmtList(channel, program->stmtList, depth);
}

void printfAstProgram(AstProgram *program) {
  if (program == NULL) {
    printf("[No program]\n");
  } else {
    fprintfAstProgram(stdout, program, 0);
  }
}
