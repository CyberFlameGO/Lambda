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

#ifndef _LAMBDA_PARSER_H_
#define _LAMBDA_PARSER_H_

#include "ast.h"
#include "lexer.h"

typedef struct lambda_parser {
  lambda_error_handler_t *error_handler;

  lambda_lexer_t *lexer;

  lambda_lexer_t *current_token;
  lambda_lexer_t *peek_token;
} lambda_parser_t;

extern void init_lambda_parser(lambda_parser_t *p, lambda_lexer_t *l);

extern void lambda_parser_parse(lambda_ast_t *ast, lambda_parser_t *p);

#endif /* _LAMBDA_PARSER_H_ */