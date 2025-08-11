#include "lexer.h"
#include "charvec.h"
#include "mystring.h"
#include <stdint.h>
#include <stdlib.h>

const char *TOKEN_TYPE_STRING[] = {FOREACH_TOKEN(GENERATE_TOKEN_STRING)};
const char *KEYWORD_STRING[] = {FOREACH_KEYWORD(GENERATE_KEYWORD_STRING)};

TokenList *tklCreate() {
  TokenList *result = (TokenList *)malloc(sizeof(TokenList));
  result->head = NULL;
  result->tail = NULL;
  result->length = 0;
  return result;
}

void tklAppend(TokenList *list, TokenType type, void *value) {
  Token *tok = (Token *)malloc(sizeof(Token));
  tok->type = type;
  tok->next = NULL;
  tok->prev = NULL;

  switch (type) {
  case TokenMinus:
  case TokenAdd:
  case TokenMult:
  case TokenDiv:
  case TokenMod:
  case TokenOpenParent:
  case TokenCloseParent:
  case TokenSemi:
  case TokenNewline:
  case TokenEndOfFile:
  case TokenOpenBracket:
  case TokenCloseBracket:
    tok->value = NULL;
    break;
  case TokenIdentifier:
  case TokenInteger:
  case TokenFloating:
  case TokenSpace:
  case TokenKeyword:
    tok->value = value;
    break;
  default:
    fprintf(stderr, "tklAppend: Unknow Token type: %d.\n", type);
    free(tok);
    return;
  }

  if (list->length == 0) {
    list->head = tok;
  } else {
    tok->prev = list->tail;
    list->tail->next = tok;
  }
  list->tail = tok;
  list->length++;
}

void freeToken(Token *tok, bool freeValue) {
  switch (tok->type) {
  case TokenIdentifier:
  case TokenInteger:
  case TokenFloating:
  case TokenKeyword:
    if (freeValue) {
      free(tok->value);
    }
    break;
  default:
    break;
  }
  free(tok);
}

void freeTkl(TokenList *list, bool freeTokensValue) {
  if (list->length == 0) {
    free(list);
    return;
  }
  Token *tok = list->head;
  while (tok != NULL) {
    Token *toFree = tok;
    tok = tok->next;
    freeToken(toFree, freeTokensValue);
  }
  free(list);
}

void fprintfToken(FILE *channel, Token *tok) {
  if (tok == NULL) {
    fprintf(stderr, "NULL\n");
    return;
  }
  fprintf(channel, "Token(%s", TOKEN_TYPE_STRING[tok->type]);
  if (tok->type == TokenInteger) {
    int value = *((int *)tok->value);
    fprintf(channel, "(%d)", value);
  } else if (tok->type == TokenFloating) {
    double value = *((double *)tok->value);
    fprintf(channel, "(%.4f)", value);
  } else if (tok->type == TokenIdentifier || tok->type == TokenKeyword) {
    fprintf(channel, "(%s)", (char *)tok->value);
  } else if (tok->type == TokenSpace) {
    fprintf(channel, "(%ld)", (intptr_t)tok->value);
  }
  fprintf(channel, ")");
}

void printfToken(Token *tok) { fprintfToken(stdout, tok); }

void printfTkl(TokenList *tkl) {
  if (tkl->length == 0) {
    printf("TokenList(length=0)\n");
    return;
  }

  printf("TokenList(length=%d,\n", tkl->length);
  for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
    printf("  ");
    printfToken(tok);
    printf(",\n");
  }
  printf(")\n");
}

TokenList *tokenizeFromFile(FILE *file) {
  TokenList *tkl = tklCreate();

  while (true) {
    char c = fgetc(file);
    if (c == EOF) {
      tklAppend(tkl, TokenEndOfFile, NULL);
      break;
    } else if (c == ' ') {
      intptr_t value = 0;
      while (c == ' ') {
        value++;
        c = fgetc(file);
      }
      ungetc(c, file);
      tklAppend(tkl, TokenSpace, (char *)value);
    } else if (c == '#') {
      while (c != '\n') {
        c = fgetc(file);
        if (c == EOF) {
          tklAppend(tkl, TokenEndOfFile, NULL);
          break;
        }
      }
      tklAppend(tkl, TokenNewline, NULL);
    } else if (c == '\n') {
      tklAppend(tkl, TokenNewline, NULL);
    } else if (c == '(') {
      tklAppend(tkl, TokenOpenParent, NULL);
    } else if (c == ')') {
      tklAppend(tkl, TokenCloseParent, NULL);
    } else if (c == '-') {
      tklAppend(tkl, TokenMinus, NULL);
    } else if (c == '+') {
      tklAppend(tkl, TokenAdd, NULL);
    } else if (c == '*') {
      tklAppend(tkl, TokenMult, NULL);
    } else if (c == '/') {
      tklAppend(tkl, TokenDiv, NULL);
    } else if (c == '%') {
      tklAppend(tkl, TokenMod, NULL);
    } else if (c == ';') {
      tklAppend(tkl, TokenSemi, NULL);
    } else if (c == '{') {
      tklAppend(tkl, TokenOpenBracket, NULL);
    } else if (c == '}') {
      tklAppend(tkl, TokenCloseBracket, NULL);
    } else if (isLetter(c)) {
      CharVec *cv = cvCreate();
      while (isAlphanum(c)) {
        appendCv(cv, c);
        c = fgetc(file);
      }
      ungetc(c, file);
      char *word = cvToString(cv);
      bool isKeyword = false;

      for (int i = 0; (i < KeywordCount) && !isKeyword; i++) {
        if (stringCmp(word, KEYWORD_STRING[i]) == 0) {
          tklAppend(tkl, TokenKeyword, word);
          isKeyword = true;
        }
      }

      if (!isKeyword) {
        tklAppend(tkl, TokenIdentifier, word);
      }
    } else if (isNum(c) || c == '.') {
      bool isFloating = c == '.';
      CharVec *cv = cvCreate();
      bool firstIter = true;
      while (isNum(c) || c == '.') {
        if (c == '.' && !firstIter) {
          if (isFloating) {
            break;
          } else {
            isFloating = true;
          }
        }
        firstIter = false;
        appendCv(cv, c);
        c = fgetc(file);
      }
      ungetc(c, file);
      if (isFloating) {
        double *value = cvToFloating(cv);
        tklAppend(tkl, TokenFloating, (void *)value);
      } else {
        int *value = cvToInteger(cv);
        tklAppend(tkl, TokenInteger, (void *)value);
      }
    } else {
      fprintf(
          stderr,
          "Unable to lex this character: \"%c\"=0x%02hhx. Aborting lexing.\n",
          c, c);
      freeTkl(tkl, true);
      return NULL;
    }
  }

  return tkl;
}

bool removeSpaces(TokenList *tkl) {
  if (tkl->length == 0) {
    return false;
  }

  Token *tok = tkl->head;
  if (tok->type == TokenSpace) {
    return true;
  }

  while (tok != NULL) {
    if (tok->type == TokenSpace) {
      if (tok->prev != NULL) {
        // printf("Setting previous ");
        // printfToken(tok->prev);
        // printf(" to ");
        // printfToken(tok->next);
        // printf("\n");
        tok->prev->next = tok->next;
      }
      if (tok->next != NULL) {
        // printf("Setting next ");
        // printfToken(tok->next);
        // printf(" to ");
        // printfToken(tok->prev);
        // printf("\n");
        tok->next->prev = tok->prev;
      }
      if (tkl->tail == tok) {
        tkl->tail = tok->prev;
      }
      Token *toFree = tok;
      tok = tok->next;
      tkl->length--;
      freeToken(toFree, false);
    } else {
      tok = tok->next;
    }
  }

  return false;
}

Token *tklPeek(TokenList *tkl) { return tkl->head; }

void tklPop(TokenList *tkl) {
  if (tkl->length == 0) {
    fprintf(stderr, "tklPop: TokenList empty, poped an empty list.\n");
    return;
  }
  tkl->length--;
  Token *toPop = tkl->head;
  tkl->head = toPop->next;
  if (tkl->head != NULL) {
    tkl->head->prev = NULL;
  }
  freeToken(toPop, true);
}