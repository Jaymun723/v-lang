#pragma once
#include <stdbool.h>
#include <stdio.h>

#define GENERATE_TOKEN_ENUM(ENUM) Token##ENUM,
#define GENERATE_TOKEN_STRING(STRING) #STRING,

#define FOREACH_TOKEN(TOKEN)                                                   \
  TOKEN(Integer)                                                               \
  TOKEN(Floating)                                                              \
  TOKEN(Minus)                                                                 \
  TOKEN(Add)                                                                   \
  TOKEN(Mult)                                                                  \
  TOKEN(Div)                                                                   \
  TOKEN(Mod)                                                                   \
  TOKEN(Identifier)                                                            \
  TOKEN(OpenParent)                                                            \
  TOKEN(CloseParent)                                                           \
  TOKEN(Semi)                                                                  \
  TOKEN(Space)                                                                 \
  TOKEN(Newline)                                                               \
  TOKEN(EndOfFile)

typedef enum TokenType {
  FOREACH_TOKEN(GENERATE_TOKEN_ENUM) TokenTypeCount
} TokenType;

static const char *TOKEN_TYPE_STRING[] = {FOREACH_TOKEN(GENERATE_TOKEN_STRING)};

// #define FOREACH_KEYWORD(KEYWORD) KEYWORD(print)

// typedef enum KeywordType {
//   FOREACH_KEYWORD(GENERATE_ENUM_WITH_PREFIX_KW) KeywordTypeCount
// } KeywordType;

// static const char *KEYWORD_TYPE_STRING[] =
// {FOREACH_KEYWORD(GENERATE_STRING)};

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

void freeToken(Token *tok, bool freeValue);
void freeTkl(TokenList *list, bool freeTokensString);

void fprintfToken(FILE *channel, Token *tok);
void printfToken(Token *tok);
void printfTkl(TokenList *tkl);

TokenList *tokenize(const char *sourceCode);

/**
 * Returns true if it start with a space.
 */
bool removeSpaces(TokenList *tkl);

Token *tklPeak(TokenList *tkl);
void tklPop(TokenList *tkl);