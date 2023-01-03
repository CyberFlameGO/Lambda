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

#ifndef _LAMBDA_LEXER_H_
#define _LAMBDA_LEXER_H_

#include "error_handler.h"
#include "location.h"
#include "token.h"

typedef struct lambda_lexer {
  const char *filename;
  char *source;
  size_t source_len;

  lambda_char_location_t *location;

  lambda_line_offsets_t *line_offsets;

  lambda_error_handler_t *error_handler;
  bool error_handling_enabled;
} lambda_lexer_t;

extern void init_lambda_lexer(lambda_lexer_t *l, const char *filename,
                              char *source);

extern void lambda_lexer_enable_error_handling(lambda_lexer_t *l,
                                               lambda_error_handler_t *h);

extern void lambda_lexer_next_token(lambda_token_t *t, lambda_lexer_t *l);

#endif /* _LAMBDA_LEXER_H_ */