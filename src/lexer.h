#pragma once
#include <stdbool.h>
#include <stdio.h>

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_ENUM_WITH_PREFIX_KW(ENUM) kw##ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define FOREACH_TOKEN(TOKEN)                                                   \
  TOKEN(Keyword)                                                               \
  TOKEN(Integer)                                                               \
  TOKEN(Floating)                                                              \
  TOKEN(OpenParent)                                                            \
  TOKEN(CloseParent)                                                           \
  TOKEN(Word)                                                                  \
  TOKEN(Space)                                                                 \
  TOKEN(Newline)                                                               \
  TOKEN(EndOfFile)

typedef enum TokenType {
  FOREACH_TOKEN(GENERATE_ENUM) TokenTypeCount
} TokenType;

static const char *TOKEN_TYPE_STRING[] = {FOREACH_TOKEN(GENERATE_STRING)};

#define FOREACH_KEYWORD(KEYWORD) KEYWORD(print)

typedef enum KeywordType {
  FOREACH_KEYWORD(GENERATE_ENUM_WITH_PREFIX_KW) KeywordTypeCount
} KeywordType;

static const char *KEYWORD_TYPE_STRING[] = {FOREACH_KEYWORD(GENERATE_STRING)};

typedef struct Token {
  TokenType type;
  void *value;

  struct Token *prev;
  struct Token *next;
} Token;

typedef struct TokenList {
  Token *head;
  Token *tail;

  int length;
} TokenList;

void tokenFree(Token *tok, bool freeValue);
void tklFree(TokenList *list, bool freeTokensString);

void tokenFprintf(FILE *channel, Token *tok);
void tokenPrintf(Token *tok);
void tklPrint(TokenList *tkl);

TokenList *tokenize(const char *sourceCode);