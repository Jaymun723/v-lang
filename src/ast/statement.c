#include "statement.h"
#include <stdlib.h>

AstStatement *parseAstStatement(TokenList *tkl) {
  AstStatement *statement = (AstStatement *)malloc(sizeof(AstStatement));
  Token *tok = tklPeek(tkl);
  if (!tok)
    return NULL;

  if (tok->type == TokenIdentifier) {
    statement->type = AstStmtFuncCall;
    AstExpr *funcCall = parseUnaryExpr(tkl);

    if (!funcCall) {
      return NULL;
    }

    tok = tklPeak(tkl);
    if (tok->type != TokenSemi) {
      fprintf(stderr, "parseAstStatement: Expected ';' after function call.\n");
      freeAstExpr(funcCall);
      freeAstStatement(statement);
      return NULL;
    }
    tklPop(tkl);

    statement->funcCall = funcCall;

    return statement;
  }

  fprintf(stderr, "parseAstStatement: Unexpected token in statement\n");
  freeAstStatement(statement);
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

void fprintfAstStatement(FILE *channel, AstStatement *statement) {
  fprintf(channel, "Statement(");
  if (statement->type == AstStmtFuncCall) {
    fprintf(channel, "FuncCall: ");
    fprintfAstExpr(channel, statement->funcCall);
  }
  if (statement->next != NULL) {
    fprintf(channel, "), ");
    fprintfAstStatement(channel, statement->next);
  }
  fprintf(channel, ")");
}
