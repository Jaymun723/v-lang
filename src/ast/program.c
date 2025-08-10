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

AstProgram *createProgram() {
  AstProgram *program = (AstProgram *)malloc(sizeof(AstProgram));
  program->statementHead = NULL;
  program->statementTail = NULL;
  program->numStatements = 0;
  return program;
}

AstProgram *parseAstProgram(TokenList *tkl) {
  AstProgram *program = createProgram();
  Token *tok = tklPeek(tkl);
  while (tok != NULL && tok->type != TokenEndOfFile) {
    if (tok->type == TokenNewline) {
      tklPop(tkl);
    } else {
      AstStatement *statement = parseAstStatement(tkl);
      if (statement == NULL) {
        freeAstProgram(program);
        return NULL;
      }
      addAstStatement(program, statement);
    }
    tok = tklPeek(tkl);
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

void fprintfAstProgram(FILE *channel, AstProgram *program, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, "Program(numStatements=%d\n", program->numStatements);
  if (program->statementHead) {
    fprintfAstStatement(channel, program->statementHead, depth + 1);
  }
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, ")\n");
}

void printfAstProgram(AstProgram *program) {
  if (program == NULL) {
    printf("[No program]\n");
  } else {
    fprintfAstProgram(stdout, program, 0);
  }
}
