#include "lexer.h"
#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>

TokenList *tkl_create() {
  TokenList *result = (TokenList *)malloc(sizeof(TokenList));
  result->head = NULL;
  result->tail = NULL;
  result->length = 0;
  return result;
}

void tkl_append(TokenList *list, TokenType type, const char *source, int len) {
  Token *tok = (Token *)malloc(sizeof(Token));
  tok->type = type;
  tok->next = NULL;
  tok->prev = NULL;
  tok->value = stringCopyN(source, len);
  if (list->length == 0) {
    list->head = tok;
  } else {
    tok->prev = list->tail;
    list->tail->next = tok;
  }
  list->tail = tok;
  list->length++;
  printf("tkl_append(%d, %s)\n", type, tok->value);
}

void tkl_pre_append(TokenList *list, TokenType type, const char *source,
                    int len) {
  Token *tok = (Token *)malloc(sizeof(Token));
  tok->type = type;
  tok->next = NULL;
  tok->prev = NULL;
  if (len >= 0) {
    tok->value = stringCopyN(source, len);
  } else {
    tok->value = source;
  }

  if (list->length == 0) {
    list->head = tok;
    list->tail = tok;
  } else if (list->length == 1) {
    list->head = tok;
    tok->next = list->tail;
    list->tail->prev = tok;
  } else {
    tok->prev = list->tail->prev;
    tok->next = list->tail;
    list->tail->prev->next = tok;
    list->tail->prev = tok;
  }

  list->length++;
  printf("tkl_pre_append(%d, %s)\n", type, tok->value);
}

void tkl_free(TokenList *list, bool freeTokensString) {
  if (list->length == 0) {
    free(list);
    return;
  }
  Token *tok = list->head;
  while (tok != NULL) {
    if (freeTokensString) {
      free(tok->value);
    }
    Token *toFree = tok;
    tok = tok->next;
    free(toFree);
  }
  free(list);
}

TokenList *toWords(const char *sourceCode) {
  int length = stringLength(sourceCode);
  TokenList *tkl = tkl_create();

  int wordStart = 0;
  for (int i = 0; i < length; i++) {
    char c = sourceCode[i];
    if (c == ' ' || c == '\n') {
      if (wordStart != i) {
        tkl_append(tkl, Word, &sourceCode[wordStart], i - wordStart);
      }
      wordStart = i + 1;
    }
  }
  if (wordStart != length) {
    tkl_append(tkl, Word, &sourceCode[wordStart], length - wordStart);
  }

  return tkl;
}

void tkl_print(TokenList *tkl) {
  if (tkl->length == 0) {
    printf("TokenList(length=0)\n");
    return;
  }

  printf("TokenList(length=%d,\n", tkl->length);
  for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
    printf("  Token(value=\"%s\")", tok->value);
    if (tok->next == NULL) {
      printf(")\n");
    } else {
      printf(",\n");
    }
    // if (tok->next == NULL && tok->prev == NULL) {
    //   printf(" Token(value=\"%s\"))\n", tok->value);
    // } else if (tok->next == NULL) {
    //   printf(" Token(value=\"%s\", prev=\"%s\"))\n", tok->value,
    //          tok->prev->value);
    // } else if (tok->prev == NULL) {
    //   printf(" Token(value=\"%s\", next=\"%s\"),", tok->value,
    //          tok->next->value);
    // } else {
    //   printf(" Token(value=\"%s\", prev=\"%s\", next=\"%s\"),", tok->value,
    //          tok->prev->value, tok->next->value);
    // }
  }
}

TokenList *tokenize_words(const char *sourceCode) {
  TokenList *tkl = toWords(sourceCode);

  for (Token *tok = tkl->head; tok != NULL; tok = tok->next) {
    char *value = tok->value;
    if (0 == stringCmp(value, "(")) {
      tok->type = OpenParent;
    } else if (0 == stringCmp(value, ")")) {
      tok->type = CloseParent;
    } else if (isInteger(value)) {
      tok->type = Integer;
    } else if (isFloating(value)) {
      tok->type = Floating;
    } else {
      for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (0 == stringCmp(value, KEYWORDS[i])) {
          tok->type = Keyword;
          break;
        }
      }
    }
    if (tok->type == Word) {
      fprintf(stderr,
              "Unknow sequence in source code: \"%s\". Abort compilation",
              value);
      tkl_free(tkl, true);
      return NULL;
    }
  }

  return tkl;
}

TokenList *tokenize(const char *sourceCode) {
  printf("Reading: \"%s\"\n", sourceCode);
  int currentIndex = 0;
  int sequenceStart = 0;

  bool readingWord = false;
  // 0 -> not reading, 1 -> reading integer, 2 -> reading floating
  int readingNumber = 0;

  TokenList *tkl = tkl_create();

  while (sourceCode[currentIndex] != '\0') {
    printf("---\n");
    printf("currentIndex=%d\n", currentIndex);
    printf("sequenceStart=%d\n", sequenceStart);
    printf("readingWord=%d\n", readingWord);
    printf("readingNumber=%d\n", readingNumber);

    char c = sourceCode[currentIndex];
    printf("c=%c\n", c);
    bool closeMultiChar = false;
    // Single character
    if (c == '(') {
      tkl_append(tkl, OpenParent, &sourceCode[currentIndex], 1);
      closeMultiChar = true;
      if (sequenceStart == currentIndex) {
        sequenceStart++;
      }
    } else if (c == ')') {
      tkl_append(tkl, CloseParent, &sourceCode[currentIndex], 1);
      closeMultiChar = true;
      if (sequenceStart == currentIndex) {
        sequenceStart++;
      }
      // } else if (c == ' ' || c == '\n') {
      //   closeMultiChar = true;
    } else if (c == ' ') {
      tkl_append(tkl, Space, &sourceCode[currentIndex], 1);
      closeMultiChar = true;
    } else if (c == '\n') {
      tkl_append(tkl, Newline, &sourceCode[currentIndex], 1);
      closeMultiChar = true;
    } else if (c == '.') {
      if (readingWord) {
        fprintf(stderr, "Unexpected \".\" encountered in parsing a word. "
                        "Aborting compilation.\n");
        tkl_free(tkl, true);
        return NULL;
      }
      if (readingNumber == 0 || readingNumber == 1) {
        readingNumber = 2;
      } else if (readingNumber == 2) {
        fprintf(stderr, "Unexpected \".\" encountered in parsing a floating "
                        "point number. Aborting compilation.\n");
        tkl_free(tkl, true);
        return NULL;
      }
    } else if (isAlpha(c) && !readingWord) {
      if (readingNumber > 0) {
        fprintf(stderr, "Bizzare bazzar.\n");
        readingNumber = 0;
      }
      readingWord = true;
      sequenceStart = currentIndex;
    } else if (isNum(c) && !readingNumber && !readingWord) {
      readingNumber = 1;
    } else if (!isAlphanum(c)) {
      fprintf(stderr, "Unexpected charater \"%c\". Aborting compilation.\n", c);
      tkl_free(tkl, true);
      return NULL;
    }

    printf("closeMultiChar=%d && sequenceStart < currentIndex=%d\n",
           closeMultiChar, sequenceStart < currentIndex);

    if (closeMultiChar && sequenceStart < currentIndex) {
      if (readingNumber == 1) {
        tkl_pre_append(tkl, Integer, &sourceCode[sequenceStart],
                       currentIndex - sequenceStart);
      } else if (readingNumber == 2) {
        tkl_pre_append(tkl, Floating, &sourceCode[sequenceStart],
                       currentIndex - sequenceStart);
      } else if (readingWord) {
        char *word = stringCopyN(&sourceCode[sequenceStart],
                                 currentIndex - sequenceStart);
        bool isKeyword = false;

        for (int i = 0; i < KEYWORD_COUNT; i++) {
          if (stringCmp(word, KEYWORDS[i]) == 0) {
            tkl_pre_append(tkl, Keyword, word, -1);
            isKeyword = true;
            break;
          }
        }

        if (!isKeyword) {
          tkl_pre_append(tkl, Word, word, -1);
        }
      } else {
        char *sequence = stringCopyN(&sourceCode[sequenceStart],
                                     currentIndex - sequenceStart);
        fprintf(stderr,
                "Unable to lex sequence \"%s\". Aborting compilation.\n",
                sequence);
        free(sequence);
        tkl_free(tkl, true);
        return NULL;
      }

      readingNumber = 0;
      readingWord = false;
      sequenceStart = currentIndex + 1;
    }

    if (readingNumber > 0 && (currentIndex + 1 - sequenceStart > 0)) {
      char *nbBuffer = stringCopyN(&sourceCode[sequenceStart],
                                   currentIndex + 1 - sequenceStart);
      printf("number buffer: %s [%d]\n", nbBuffer,
             currentIndex + 1 - sequenceStart);
      free(nbBuffer);
    }
    if (readingWord && (currentIndex + 1 - sequenceStart > 0)) {
      char *wdBuffer = stringCopyN(&sourceCode[sequenceStart],
                                   currentIndex + 1 - sequenceStart);
      printf("word buffer: %s [%d]\n", wdBuffer,
             currentIndex + 1 - sequenceStart);
      free(wdBuffer);
    }

    printf("Intermediate tkl: ");
    tkl_print(tkl);

    currentIndex++;

    // if (currentIndex == 5) {
    //   int *a = NULL;
    //   printf("%d", *a);
    // }
  }

  return tkl;
}
