#include "lexer.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define current_chr(l) l->source[l->location->index]

static void lambda_lexer_advance(lambda_lexer_t *l) {
  if (current_chr(l) == '\n') {
    l->location->line++;
    l->location->column = 0;
  } else {
    l->location->column++;
  }

  l->location->index++;
}

extern void init_lambda_lexer(lambda_lexer_t *l, const char *filename,
                              char *source) {
  l->filename = filename;
  l->source = source;
  l->source_len = strlen(source);

  l->location = malloc(sizeof(lambda_char_location_t));
  init_lambda_char_location(l->location,
                            /* index */ 0,
                            /* line */ 1,
                            /* column */ 0);
}

static void lambda_lexer_next_identifier_token(lambda_token_t *t,
                                               lambda_lexer_t *l) {
  lambda_char_location_t *start_location =
      malloc(sizeof(lambda_char_location_t));
  copy_lambda_char_location(l->location, start_location);

  while ((current_chr(l) >= 'a' && current_chr(l) <= 'z') ||
         (current_chr(l) == '_') ||
         (current_chr(l) >= '0' && current_chr(l) <= '9')) {
    lambda_lexer_advance(l);
  }

  lambda_char_location_t *end_location = malloc(sizeof(lambda_char_location_t));
  copy_lambda_char_location(l->location, end_location);

  size_t size = end_location->index - start_location->index;

  char *identifier_buffer = malloc(size + 1);
  lambda_substring(l->source, identifier_buffer, start_location->index, size);

  lambda_block_location_t *bl = malloc(sizeof(lambda_block_location_t));
  init_lambda_block_location(bl, start_location, end_location);

  init_lambda_token(t, LAMBDA_IDENTIFIER_TOKEN_KIND, bl, identifier_buffer);
}

#define one_char_token(c, k)                                                   \
  lambda_block_location_t *bl = malloc(sizeof(lambda_block_location_t));       \
  bl->end = malloc(sizeof(lambda_char_location_t));                            \
  lambda_char_location_t *start_location =                                     \
      malloc(sizeof(lambda_char_location_t));                                  \
  copy_lambda_char_location(l->location, start_location);                      \
  init_lambda_block_location_from_char_location(bl, start_location);           \
  lambda_lexer_advance(l);                                                     \
  char *s = malloc(2);                                                         \
  s[0] = c;                                                                    \
  s[1] = '\0';                                                                 \
  init_lambda_token(t, k, bl, s);

#define two_char_token(s, k)                                                   \
  lambda_block_location_t *bl = malloc(sizeof(lambda_block_location_t));       \
  bl->end = malloc(sizeof(lambda_char_location_t));                            \
  lambda_char_location_t *start_location =                                     \
      malloc(sizeof(lambda_char_location_t));                                  \
  copy_lambda_char_location(l->location, start_location);                      \
  init_lambda_two_chars_block_location(bl, start_location);                    \
  lambda_lexer_advance(l);                                                     \
  lambda_lexer_advance(l);                                                     \
  init_lambda_token(t, k, bl, s);

#define eof_token()                                                            \
  lambda_block_location_t *bl = malloc(sizeof(lambda_block_location_t));       \
  bl->end = malloc(sizeof(lambda_char_location_t));                            \
  lambda_char_location_t *start_location =                                     \
      malloc(sizeof(lambda_char_location_t));                                  \
  copy_lambda_char_location(l->location, start_location);                      \
  init_lambda_block_location_from_char_location(bl, start_location);           \
  lambda_lexer_advance(l);                                                     \
  char *s = malloc(1);                                                         \
  s[0] = '\0';                                                                 \
  init_lambda_token(t, LAMBDA_EOF_TOKEN_KIND, bl, s);

static void lambda_lexer_skip_whitespaces(lambda_lexer_t *l) {
  while (isspace(current_chr(l))) {
    lambda_lexer_advance(l);
  }
}

static char lambda_lexer_lookahead(lambda_lexer_t *l) {
  if (l->location->index + 1 >= l->source_len)
    return 0;

  return l->source[l->location->index + 1];
}

extern void lambda_lexer_next_token(lambda_token_t *t, lambda_lexer_t *l) {
  lambda_lexer_skip_whitespaces(l);

  switch (current_chr(l)) {
  case '(': {
    one_char_token('(', LAMBDA_LPAREN_TOKEN_KIND);
    return;
  }
  case ')': {
    one_char_token(')', LAMBDA_RPAREN_TOKEN_KIND);
    return;
  }
  case '.': {
    one_char_token('.', LAMBDA_PERIOD_TOKEN_KIND);
    return;
  }
  case '$': {
    one_char_token('$', LAMBDA_DOLLAR_TOKEN_KIND);
    return;
  }
  case '=': {
    one_char_token('=', LAMBDA_EQUAL_TOKEN_KIND);
    return;
  }
  case '\\': {
    one_char_token('\\', LAMBDA_LAMBDA_TOKEN_KIND);
    return;
  }
  case '\0': {
    eof_token();
    return;
  }
  default: {
    if (current_chr(l) == '_' ||
        (current_chr(l) >= 'a' && current_chr(l) <= 'z')) {
      lambda_lexer_next_identifier_token(t, l);
      return;
    } else if (current_chr(l) == (char)206 &&
               lambda_lexer_lookahead(l) == (char)187) {
      char *s = malloc(3);
      s[0] = 206;
      s[1] = 187;
      s[2] = '\0';
      two_char_token(s, LAMBDA_LAMBDA_TOKEN_KIND);
      return;
    } else if (current_chr(l) == '-' && lambda_lexer_lookahead(l) == '>') {
      char *s = malloc(3);
      s[0] = '-';
      s[1] = '>';
      s[2] = '\0';
      two_char_token(s, LAMBDA_RIGHT_ARROW_TOKEN_KIND);
      return;
    }
  }
  }

  one_char_token(current_chr(l), LAMBDA_ERROR_TOKEN_KIND);
  lambda_lexer_advance(l);
}