#include "statement.h"
#include <stdlib.h>

AstStatement *createStatementFuncCall(AstExpr *funcCall) {
  AstStatement *statement = (AstStatement *)malloc(sizeof(AstStatement));
  statement->type = AstStmtFuncCall;
  statement->next = NULL;
  statement->funcCall = funcCall;

  return statement;
}

AstStatement *parseAstStatement(TokenList *tkl) {
  Token *tok = tklPeek(tkl);
  if (!tok) {
    return NULL;
  }

  if (tok->type == TokenIdentifier) {
    AstExpr *funcCall = parseUnaryExpr(tkl);

    if (!funcCall) {
      return NULL;
    }

    tok = tklPeek(tkl);
    if (tok->type != TokenSemi) {
      fprintf(stderr, "parseAstStatement: Expected ';' after function call.\n");
      freeAstExpr(funcCall);
      return NULL;
    }
    tklPop(tkl);

    return createStatementFuncCall(funcCall);
  }

  fprintf(stderr, "parseAstStatement: Unexpected token in statement\n");
  return NULL;
}

void freeAstStatement(AstStatement *statement) {
  if (statement->type == AstStmtFuncCall) {
    freeAstExpr(statement->funcCall);
  }
  if (statement->next != NULL) {
    freeAstStatement(statement->next);
  }
  free(statement);
}

void fprintfAstStatement(FILE *channel, AstStatement *statement, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, "Statement(");
  if (statement->type == AstStmtFuncCall) {
    fprintf(channel, "FuncCall:\n");
    fprintfAstExpr(channel, statement->funcCall, depth);
  }
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  if (statement->next != NULL) {
    fprintf(channel, "),\n");
    fprintfAstStatement(channel, statement->next, depth);
  }
  fprintf(channel, ")\n");
}
