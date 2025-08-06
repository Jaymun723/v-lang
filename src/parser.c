#include "parser.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Token *tklPop(TokenList *tkl) {
  // assumes tkl is not empty
  if (tkl->length == 0) {
    fprintf(stderr, "tklPop: TokenList is empty. Aborting parsing.\n");
    return NULL;
  }
  Token *res = tkl->head;
  if (tkl->length == 1) {
    tkl->tail = NULL;
    tkl->head = NULL;
    tkl->length = 0;
  } else {
    tkl->head = res->next;
    res->next->prev = NULL;
    res->next = NULL;
    tkl->length--;
  }
  return res;
}

AstNode *astCreate(AstType type, void *value, AstNode *first_child,
                   AstNode *second_child) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = type;
  if (type == floatingLitteral) {
    double *dvalue = malloc(sizeof(double));
    *dvalue = *(double *)value;
    node->value = (void *)dvalue;
  } else if (type == integerLitteral) {
    int *ivalue = malloc(sizeof(int));
    *ivalue = *(int *)value;
    node->value = (void *)ivalue;
  } else {
    node->value = NULL;
  }
  node->first_child = first_child;
  node->second_child = second_child;
  return node;
}

void astFree(AstNode *node) {
  if (node != NULL) {
    if (node->type == printStatement) {
      astFree(node->first_child);
      free(node);
    } else if (node->type == integerLitteral ||
               node->type == floatingLitteral) {
      free(node->value);
      free(node);
    }
  }
}

void astFprintf(FILE *channel, AstNode *node, int depth) {
  if (node == NULL) {
    printf("ya r gro\n");
    return;
  }
  for (int i = 0; i < depth; i++) {
    fprintf(channel, "  ");
  }
  fprintf(channel, "AstNode(type=%s, ", AST_TYPE_STRING[node->type]);
  if (node->type == printStatement) {
    fprintf(channel, "first_child=\n");
    astFprintf(channel, node->first_child, depth + 1);
    for (int i = 0; i < depth; i++) {
      fprintf(channel, "  ");
    }
    fprintf(channel, ")\n");
  } else if (node->type == integerLitteral) {
    int value = *(int *)node->value;
    fprintf(channel, "value=%d)\n", value);
  } else if (node->type == floatingLitteral) {
    double value = *(double *)node->value;
    fprintf(channel, "value=%f)\n", value);
  }
}

void astPrintroot(AstNode *node) { astFprintf(stdout, node, 0); }

bool read_parent(TokenList *tkl, bool open, bool discard) {
  Token *tok = tklPop(tkl);
  if (open && (tok->type != OpenParent)) {
    fprintf(stderr, "Expected \"(\". Aborting parsing.\n");
    return false;
  } else if (!open && (tok->type != CloseParent)) {
    fprintf(stderr, "Expected \")\". Aborting parsing.\n");
    return false;
  }
  if (discard) {
    tokenFree(tok, true);
  }
  return true;
}

AstNode *read_expression(TokenList *tkl, bool discard) {
  Token *tok = tklPop(tkl);
  AstNode *node = NULL;

  if (tok->type == OpenParent) {
    node = read_expression(tkl, discard);
    if (!read_parent(tkl, false, discard)) {
      astFree(node);
      return NULL;
    }
  } else if (tok->type == Floating) {
    // check for binary operator

    node = astCreate(floatingLitteral, tok->value, NULL, NULL);

  } else if (tok->type == Integer) {
    // check for binary operator

    node = astCreate(integerLitteral, tok->value, NULL, NULL);
  } else {
    fprintf(stderr, "read_expression: Unexpected token encoutered: ");
    tokenFprintf(stderr, tok);
    fprintf(stderr, ". Aborting parsing.\n");
    return NULL;
  }

  if (discard) {
    tokenFree(tok, true);
  }

  return node;
}

AstNode *read_statement(TokenList *tkl, bool discard) {
  Token *tok = tklPop(tkl);
  AstNode *node = NULL;

  if (tok->type == Keyword) {
    int keywordType = (intptr_t)tok->value;
    if (keywordType == kwprint) {
      if (!read_parent(tkl, true, discard)) {
        return NULL;
      }
      AstNode *expr = read_expression(tkl, discard);
      // astFprintf(stdout, expr, 0);
      if (!read_parent(tkl, false, discard)) {
        astFree(expr);
        return NULL;
      }
      // printf("yassss\n");
      node = astCreate(printStatement, NULL, expr, NULL);
    }
  } else {
    fprintf(stderr, "read_statement: Unexpected token encoutered: ");
    tokenFprintf(stderr, tok);
    fprintf(stderr, ". Aborting parsing.\n");
    return NULL;
  }

  if (discard) {
    tokenFree(tok, true);
  }
  return node;
}