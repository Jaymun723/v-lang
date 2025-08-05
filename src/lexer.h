#pragma once
#include "list.h"
#include <stdbool.h>

typedef enum TokenType {
  Keyword,
  Integer,
  Floating,
  OpenParent,
  CloseParent,
  Word,
  Identifier,
  Space,
  Newline,
} TokenType;

#define KEYWORD_COUNT 1
static const char *KEYWORDS[KEYWORD_COUNT] = {"print"};

typedef struct Token {
  TokenType type;
  char *value;

  struct Token *prev;
  struct Token *next;
} Token;

typedef struct TokenList {
  Token *head;
  Token *tail;

  int length;
} TokenList;

void tkl_free(TokenList *list, bool freeTokensString);

void tkl_print(TokenList *tkl);

TokenList *toWords(const char *sourceCode);

/**
 * Tokenize the source code. The tokens hold their own copy of the code source,
 * so it can be freed without creating errors.
 */
TokenList *tokenize_words(const char *sourceCode);

TokenList *tokenize(const char *sourceCode);