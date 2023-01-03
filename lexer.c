/*
        MIT License

        Copyright (c) 2023 Adi Salimgereyev (Vertex)

        Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
 */

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

  l->line_offsets = malloc(sizeof(lambda_line_offsets_t));

  l->location = malloc(sizeof(lambda_char_location_t));
  init_lambda_char_location(l->location,
                            /* index */ 0,
                            /* line */ 1,
                            /* column */ 0);

  l->error_handler = (void *)0;
  l->error_handling_enabled = false;
}

extern void lambda_lexer_enable_error_handling(lambda_lexer_t *l,
                                               lambda_error_handler_t *h) {
  l->error_handler = h;
  l->error_handling_enabled = true;

  lambda_error_handler_set_line_offsets_ptr(h, l->line_offsets);
}

static void lambda_lexer_next_identifier_token(lambda_token_t *t,
                                               lambda_lexer_t *l) {
  lambda_char_location_t *start_location =
      malloc(sizeof(lambda_char_location_t));
  copy_lambda_char_location(l->location, start_location);

  while ((current_chr(l) >= 'a' && current_chr(l) <= 'z') ||
         (current_chr(l) >= 'A' && current_chr(l) <= 'Z') ||
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
        (current_chr(l) >= 'a' && current_chr(l) <= 'z') ||
        (current_chr(l) >= 'A' && current_chr(l) <= 'Z')) {
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

  if (l->error_handling_enabled) {
    lambda_block_location_t *error_location =
        malloc(sizeof(lambda_block_location_t));

    lambda_char_location_t *current_location_copy =
        malloc(sizeof(lambda_char_location_t));
    copy_lambda_char_location(l->location, current_location_copy);

    init_lambda_block_location_from_char_location(error_location,
                                                  current_location_copy);

    lambda_error_handler_add_error(
        l->error_handler,
        lambda_format("unexpected character '%c'", current_chr(l)),
        error_location);
  }

  one_char_token(current_chr(l), LAMBDA_ERROR_TOKEN_KIND);
  lambda_lexer_advance(l);
}