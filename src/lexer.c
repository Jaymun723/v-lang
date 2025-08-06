#include "lexer.h"
#include "mystring.h"
#include <stdint.h>
#include <stdlib.h>

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

  if (type == EndOfFile || type == Newline || type == OpenParent ||
      type == CloseParent) {
    tok->value = NULL;
  } else if (type == Space || tok->type == Word || tok->type == Keyword ||
             tok->type == Integer || tok->type == Floating) {
    tok->value = value;
  } else {
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

void tokenFree(Token *tok, bool freeValue) {
  if (freeValue) {
    if (tok->type == Word || tok->type == Keyword || tok->type == Floating ||
        tok->type == Integer) {
      free(tok->value);
    }
  }
  free(tok);
}

void tklFree(TokenList *list, bool freeTokensValue) {
  if (list->length == 0) {
    free(list);
    return;
  }
  Token *tok = list->head;
  while (tok != NULL) {
    Token *toFree = tok;
    tok = tok->next;
    tokenFree(toFree, freeTokensValue);
  }
  free(list);
}

void tokenFprintf(FILE *channel, Token *tok) {
  if (tok == NULL) {
    fprintf(stderr, "NULL\n");
    return;
  }
  fprintf(channel, "Token(");
  if (tok->type == Space) {
    fprintf(channel, "Space(%ld)", (intptr_t)tok->value);
  } else if (tok->type == EndOfFile) {
    fprintf(channel, "<EOF>");
  } else if (tok->type == Newline) {
    fprintf(channel, "\\n");
  } else if (tok->type == OpenParent) {
    fprintf(channel, "\"(\"");
  } else if (tok->type == CloseParent) {
    fprintf(channel, "\")\"");
  } else if (tok->type == Word) {
    fprintf(channel, "Word(\"%s\")", (char *)tok->value);
  } else if (tok->type == Keyword) {
    int keyword_index = (intptr_t)tok->value;
    fprintf(channel, "%s", KEYWORD_TYPE_STRING[keyword_index]);
  } else if (tok->type == Floating) {
    double value = *((double *)tok->value);
    fprintf(channel, "%.4f", value);
  } else if (tok->type == Integer) {
    int value = *((int *)tok->value);
    fprintf(channel, "%d", value);
  } else {
    fprintf(stderr, "tokenPrint: Unknow Token type: %d.\n", tok->type);
  }
  fprintf(channel, ")");
}

void tokenPrintf(Token *tok) { tokenFprintf(stdout, tok); }

void tklPrint(TokenList *tkl) {
  if (tkl->length == 0) {
    printf("TokenList(length=0)\n");
    return;
  }

  printf("TokenList(length=%d,\n", tkl->length);
  for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
    printf("  ");
    tokenPrintf(tok);
    printf(",\n");
  }
  printf(")\n");
}

TokenList *tokenize(const char *sourceCode) {
  TokenList *tkl = tklCreate();

  int currentIndex = 0;
  int sourceCodeLenght = stringLength(sourceCode);
  while (currentIndex <= sourceCodeLenght) {
    char c = sourceCode[currentIndex];
    if (c == '\0') {
      tklAppend(tkl, EndOfFile, NULL);
      currentIndex++;
    } else if (c == ' ') {
      int start = currentIndex;
      while (sourceCode[currentIndex] == ' ') {
        currentIndex++;
      }
      intptr_t value = currentIndex - start;
      tklAppend(tkl, Space, (char *)value);
    } else if (c == '\n') {
      tklAppend(tkl, Newline, NULL);
      currentIndex++;
    } else if (c == '(') {
      tklAppend(tkl, OpenParent, NULL);
      currentIndex++;
    } else if (c == ')') {
      tklAppend(tkl, CloseParent, NULL);
      currentIndex++;
    } else if (isLetter(c)) {
      int start = currentIndex;
      while (isAlphanum(sourceCode[currentIndex])) {
        currentIndex++;
      }
      int l = currentIndex - start;
      char *word = stringCopyN(&sourceCode[start], l);

      bool isKeyword = false;
      for (int i = 0; i < KeywordTypeCount && !isKeyword; i++) {
        int cmp = stringCmp(word, KEYWORD_TYPE_STRING[i]);
        // printf("stringCmp(%s, %s)=%d\n", word, KEYWORD_TYPE_STRING[i], cmp);
        if (cmp == 0) {
          intptr_t value = (intptr_t)i;
          tklAppend(tkl, Keyword, (void *)value);
          isKeyword = true;
          free(word);
        }
      }

      if (!isKeyword) {
        tklAppend(tkl, Word, word);
      }
    } else if (isNum(c) || c == '.') {
      bool isFloating = c == '.';
      int start = currentIndex;
      while (isNum(c) || c == '.') {
        if (c == '.' && (start != currentIndex)) {
          if (isFloating) {
            break;
          } else {
            isFloating = true;
          }
        }
        currentIndex++;
        c = sourceCode[currentIndex];
      }
      if (isFloating) {
        double *value = readFloating(sourceCode, start, currentIndex);
        tklAppend(tkl, Floating, (void *)value);
      } else {
        int *value = readInteger(sourceCode, start, currentIndex);
        tklAppend(tkl, Integer, (void *)value);
      }
    } else {
      fprintf(stderr,
              "Unable to lex this character: \"%c\". Aborting lexing.\n", c);
      tklFree(tkl, true);
      return NULL;
    }
  }

  return tkl;
}
