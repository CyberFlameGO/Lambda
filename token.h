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

#ifndef _LAMBDA_TOKEN_H_
#define _LAMBDA_TOKEN_H_

#include "location.h"

typedef enum lambda_token_kind {
  LAMBDA_LPAREN_TOKEN_KIND,
  LAMBDA_RPAREN_TOKEN_KIND,
  LAMBDA_PERIOD_TOKEN_KIND,
  LAMBDA_DOLLAR_TOKEN_KIND,
  LAMBDA_EQUAL_TOKEN_KIND,
  LAMBDA_LAMBDA_TOKEN_KIND,
  LAMBDA_IDENTIFIER_TOKEN_KIND,
  LAMBDA_RIGHT_ARROW_TOKEN_KIND,
  LAMBDA_EOF_TOKEN_KIND,
  LAMBDA_ERROR_TOKEN_KIND,
} lambda_token_kind_t;

typedef struct lambda_token {
  lambda_token_kind_t kind;
  lambda_block_location_t *location;
  char *literal;
} lambda_token_t;

extern void init_lambda_token(lambda_token_t *t, lambda_token_kind_t kind,
                              lambda_block_location_t *location, char *literal);

#endif /* _LAMBDA_TOKEN_H_ */