#pragma once
#include "lexer.h"

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define FOREACH_AST(AST)                                                       \
  AST(printStatement)                                                          \
  AST(floatingLitteral)                                                        \
  AST(integerLitteral)

typedef enum AstType { FOREACH_AST(GENERATE_ENUM) AstTypeCount } AstType;

static const char *AST_TYPE_STRING[] = {FOREACH_AST(GENERATE_STRING)};

typedef struct AstNode {
  AstType type;
  void *value;
  struct AstNode *first_child;
  struct AstNode *second_child;
} AstNode;

void astFree(AstNode *node);
void astPrintroot(AstNode *node);

AstNode *read_statement(TokenList *tkl, bool discard);