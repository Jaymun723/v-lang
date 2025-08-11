#include "statement_list.h"
#include <stdlib.h>

void addAstStatement(AstStatementList *stmtList, AstStatement *statement) {
  if (stmtList->numStatement == 0) {
    stmtList->statementHead = statement;
  } else {
    stmtList->statementTail->next = statement;
  }
  stmtList->statementTail = statement;
  stmtList->numStatement++;
}

AstStatementList *createStmtList() {
  AstStatementList *stmtList =
      (AstStatementList *)malloc(sizeof(AstStatementList));
  stmtList->statementHead = NULL;
  stmtList->statementTail = NULL;
  stmtList->numStatement = 0;
  return stmtList;
}

AstStatementList *parseStmtList(TokenList *tkl) {
  AstStatementList *stmtList = createStmtList();
  Token *tok = tklPeek(tkl);
  while (tok != NULL && tok->type != TokenEndOfFile) {
    if (tok->type == TokenNewline) {
      tklPop(tkl);
    } else if (tok->type == TokenIdentifier || tok->type == TokenKeyword) {
      AstStatement *statement = parseAstStatement(tkl);
      if (statement == NULL) {
        freeAstStmtList(stmtList);
        return NULL;
      }
      addAstStatement(stmtList, statement);
    } else {
      return stmtList;
    }
    tok = tklPeek(tkl);
  }
  tklPop(tkl);
  return stmtList;
}

AstStatementList *parseBlock(TokenList *tkl) {
  Token *tok = tklPeek(tkl);
  if (tok->type != TokenOpenBracket) {
    fprintf(stderr, "parseBlock: Expected '{'.\n");
    return NULL;
  }
  tklPop(tkl);
  AstStatementList *stmtList = parseStmtList(tkl);
  tok = tklPeek(tkl);
  if (tok->type != TokenCloseBracket) {
    freeAstStmtList(stmtList);
    fprintf(stderr, "parseBlock: Expected '}'.\n");
    return NULL;
  }
  tklPop(tkl);
  return stmtList;
}

void freeAstStmtList(AstStatementList *stmtList) {
  if (stmtList->statementHead != NULL) {
    freeAstStatement(stmtList->statementHead);
  }
  free(stmtList);
}

void fprintfAstStmtList(FILE *channel, AstStatementList *stmtList, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, "StmtList(numStatements=%d\n", stmtList->numStatement);
  if (stmtList->statementHead) {
    fprintfAstStatement(channel, stmtList->statementHead, depth + 1);
  }
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, ")\n");
}

void printfAstStmtList(AstStatementList *stmtList) {
  if (stmtList == NULL) {
    printf("[No stmtList]\n");
  } else {
    fprintfAstStmtList(stdout, stmtList, 0);
  }
}
