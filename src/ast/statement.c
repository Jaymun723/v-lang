#include "statement.h"
#include "../lexer.h"
#include "../mystring.h"
#include <stdlib.h>

const char *AST_STMT_TYPE_STRING[] = {
    FOREACH_AST_STMT_TYPE(GENERATE_AST_STMT_TYPE_STRING)};

AstStatement *createStatementFuncCall(AstExpr *funcCall) {
  AstStatement *statement = (AstStatement *)malloc(sizeof(AstStatement));
  statement->type = AstStmtFuncCall;
  statement->next = NULL;
  statement->funcCall = funcCall;

  return statement;
}

AstStatement *createStatementWhile(AstExpr *condition,
                                   AstStatementList *block) {
  AstStatement *statement = (AstStatement *)malloc(sizeof(AstStatement));
  statement->type = AstStmtWhile;
  statement->next = NULL;
  statement->While.condition = condition;
  statement->While.block = block;

  return statement;
}

AstStatement *createStatementIf(AstExpr *condition, AstStatementList *block,
                                AstElseTail *tail) {
  AstStatement *statement = (AstStatement *)malloc(sizeof(AstStatement));
  statement->type = AstStmtIf;
  statement->next = NULL;
  statement->If.condition = condition;
  statement->If.block = block;
  statement->If.tail = tail;

  return statement;
}

AstStatement *parseIfStatement(TokenList *tkl) {
  Token *tok = tklPeek(tkl);
  if (stringCmp(tok->value, KEYWORD_STRING[Keyword_if]) == 0) {
    tklPop(tkl);
    tok = tklPeek(tkl);
    if (tok->type != TokenOpenParent) {
      fprintf(stderr,
              "parseIfStatement: Expected open parenthesis after 'if'.\n");
      return NULL;
    }
    // the expr will consume the parenthesis.
    AstExpr *condition = parseExpr(tkl);
    if (condition == NULL) {
      return NULL;
    }
    AstStatementList *block = parseBlock(tkl);
    if (block == NULL) {
      freeAstExpr(condition);
      return NULL;
    }
    AstElseTail *tail = parseAstElseTail(tkl);
    if (tail == NULL) {
      freeAstExpr(condition);
      freeAstStmtList(block);
      return NULL;
    }
    return createStatementIf(condition, block, tail);
  } else {
    fprintf(stderr, "parseIfStatement: Expected 'if'.\n");
    return NULL;
  }
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
  } else if (tok->type == TokenKeyword) {
    if (stringCmp(tok->value, KEYWORD_STRING[Keyword_if]) == 0) {
      return parseIfStatement(tkl);
    } else if (stringCmp(tok->value, KEYWORD_STRING[Keyword_while]) == 0) {
      tklPop(tkl);
      tok = tklPeek(tkl);
      if (tok->type != TokenOpenParent) {
        fprintf(
            stderr,
            "parseAstStatement: Expected open parenthesis after 'while'.\n");
        return NULL;
      }
      // the expr will consume the parenthesis.
      AstExpr *condition = parseExpr(tkl);
      if (condition == NULL) {
        return NULL;
      }
      AstStatementList *block = parseBlock(tkl);
      if (block == NULL) {
        freeAstExpr(condition);
        return NULL;
      }
      return createStatementWhile(condition, block);
    } else {
      fprintf(stderr, "parseAstStatement: Unexpected keyword: %s.\n",
              (char *)tok->value);
      return NULL;
    }
  }

  fprintf(stderr, "parseAstStatement: Unexpected token in statement\n");
  return NULL;
}

void freeAstStatement(AstStatement *statement) {
  if (statement->type == AstStmtFuncCall) {
    freeAstExpr(statement->funcCall);
  } else if (statement->type == AstStmtIf) {
    freeAstExpr(statement->If.condition);
    freeAstStmtList(statement->If.block);
    freeAstElseTail(statement->If.tail);
  } else if (statement->type == AstStmtWhile) {
    freeAstExpr(statement->If.condition);
    freeAstStmtList(statement->If.block);
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
  fprintf(channel, "Statement(%s,\n", AST_STMT_TYPE_STRING[statement->type]);
  if (statement->type == AstStmtFuncCall) {
    fprintfAstExpr(channel, statement->funcCall, depth + 11);
  } else if (statement->type == AstStmtIf) {
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "          condition:\n");
    fprintfAstExpr(channel, statement->If.condition, depth + 11);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "          block:\n");
    fprintfAstStmtList(channel, statement->If.block, depth + 11);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "           tail: ");
    fprintfAstElseTail(channel, statement->If.tail, depth + 11);
  } else if (statement->type == AstStmtWhile) {
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "          condition:\n");
    fprintfAstExpr(channel, statement->While.condition, depth + 11);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "          block:\n");
    fprintfAstStmtList(channel, statement->While.block, depth + 11);
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
