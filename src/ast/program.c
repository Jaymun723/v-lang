#include "program.h"
#include <stdio.h>
#include <stdlib.h>

void addAstStatement(AstProgram *program, AstStatement *statement) {
  if (program->numStatements == 0) {
    program->statementHead = statement;
  } else {
    program->statementTail->next = statement;
  }
  program->statementTail = statement;
  program->numStatements++;
}

AstProgram *parseAstProgram(TokenList *tkl) {
  AstProgram *program = (AstProgram *)malloc(sizeof(AstProgram));
  program->numStatements = 0;
  Token *tok = tklPeak(tkl);
  while (tok != NULL && tok != TokenEndOfFile) {
    if (tok->type == TokenNewline) {
      tklPop(tkl);
    } else {
      AstStatement *statement = parseAstStatement(tkl);
      if (statement == NULL) {
        freeAstProgram(program);
        return NULL;
      }
    }
    tok = tklPeak(tkl);
  }
  tklPop(tkl);
  return program;
}

void freeAstProgram(AstProgram *program) {
  if (program->statementHead != NULL) {
    freeAstStatement(program->statementHead);
  }
  free(program);
}

void fprintfAstProgram(FILE *channel, AstProgram *program) {
  fprintf(channel, "Program(numStatements=%d\n", program->numStatements);
  if (program->statementHead) {
    fprintfAstStatement(channel, program->statementHead);
  }
  fprintf(channel, ")\n");
}

void printfProgram(AstProgram *program) { fprintfAstProgram(stdout, program); }
