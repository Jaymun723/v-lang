#include "else_tail.h"
#include "../mystring.h"
#include <stdlib.h>

AstElseTail *createEmptyTail() {
  AstElseTail *tail = (AstElseTail *)malloc(sizeof(AstElseTail));
  tail->type = AstElseTailEmpty;
  return tail;
}

AstElseTail *createElifTail(AstStatement *elif) {
  AstElseTail *tail = (AstElseTail *)malloc(sizeof(AstElseTail));
  tail->type = AstElseTailElif;
  tail->elif = elif;
  return tail;
}

AstElseTail *createElseTail(AstStatementList *block) {
  AstElseTail *tail = (AstElseTail *)malloc(sizeof(AstElseTail));
  tail->type = AstElseTailElse;
  tail->block = block;
  return tail;
}

AstElseTail *parseAstElseTail(TokenList *tkl) {
  Token *tok = tklPeek(tkl);
  if (tok->type == TokenKeyword) {
    if (stringCmp(tok->value, KEYWORD_STRING[Keyword_else]) == 0) {
      tklPop(tkl);
      tok = tklPeek(tkl);
      if (tok->type == TokenKeyword &&
          stringCmp(tok->value, KEYWORD_STRING[Keyword_if]) == 0) {
        AstStatement *elif = parseIfStatement(tkl);
        if (elif == NULL) {
          return NULL;
        }
        return createElifTail(elif);
      }
      AstStatementList *block = parseBlock(tkl);
      if (block == NULL) {
        return NULL;
      }
      return createElseTail(block);
    } else {
      return createEmptyTail();
    }
  } else {
    return createEmptyTail();
  }
}

void freeAstElseTail(AstElseTail *tail) {
  if (tail->type == AstElseTailElif) {
    freeAstStatement(tail->elif);
  } else if (tail->type == AstElseTailElse) {
    freeAstStmtList(tail->block);
  }
  free(tail);
}

void fprintfAstElseTail(FILE *channel, AstElseTail *tail, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(channel, " ");
  }
  fprintf(channel, "AstElseTail(");
  if (tail->type == AstElseTailEmpty) {
    fprintf(channel, "Empty)\n");
  } else if (tail->type == AstElseTailElse) {
    fprintf(channel, "Else, {\n");
    fprintfAstStmtList(channel, tail->block, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "}\n");
  } else if (tail->type == AstElseTailElif) {
    fprintf(channel, "Elif, {\n");
    fprintfAstStatement(channel, tail->elif, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, " ");
    }
    fprintf(channel, "}\n");
  }
}
