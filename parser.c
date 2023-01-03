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

#include "parser.h"

static void lambda_parser_consume_token(lambda_parser_t *p);

static void lambda_parser_consume_remaining_tokens(lambda_parser_t *p);

static void lambda_parser_parse_statement(lambda_ast_statement_t *s,
                                          lambda_parser_t *p);

static void lambda_parser_parse_let_statement(lambda_ast_statement_t *s,
                                              lambda_parser_t *p);

static void lambda_parser_parse_expression_statement(lambda_ast_statement_t *s,
                                                     lambda_parser_t *p);

static void lambda_parser_parse_unary(lambda_ast_expression_t *e,
                                      lambda_parser_t *p);

static void lambda_parser_parse_lambda(lambda_ast_expression_t *e,
                                       lambda_parser_t *p);

static void lambda_parser_parse_parenthesised(lambda_ast_expression_t *e,
                                              lambda_parser_t *p);

static void lambda_parser_parse_apply(lambda_ast_expression_t *e,
                                      lambda_parser_t *p);

extern void init_lambda_parser(lambda_parser_t *p, lambda_lexer_t *l) {
  p->lexer = l;
  p->error_handler = l->error_handler;
  p->current_token = malloc(sizeof(lambda_token_t));
  p->peek_token = malloc(sizeof(lambda_token_t));
  lambda_parser_consume_token(p);
  lambda_parser_consume_token(p);
}

extern void lambda_parser_parse(lambda_ast_t *ast, lambda_parser_t *p);

static void lambda_parser_consume_token(lambda_parser_t *p) {
  p->current_token = p->peek_token;
  lambda_lexer_next_token(p->peek_token, p->lexer);
}

static void lambda_parser_consume_remaining_tokens(lambda_parser_t *p) {
  while (p->current_token != LAMBDA_EOF_TOKEN_KIND) {
    lambda_parser_consume_token(p);
  }
}

static void lambda_parser_parse_statement(lambda_ast_statement_t *s,
                                          lambda_parser_t *p) {}

static void lambda_parser_parse_let_statement(lambda_ast_statement_t *s,
                                              lambda_parser_t *p) {}

static void lambda_parser_parse_expression_statement(lambda_ast_statement_t *s,
                                                     lambda_parser_t *p) {}

static void lambda_parser_parse_unary(lambda_ast_expression_t *e,
                                      lambda_parser_t *p) {}

static void lambda_parser_parse_lambda(lambda_ast_expression_t *e,
                                       lambda_parser_t *p) {}

static void lambda_parser_parse_parenthesised(lambda_ast_expression_t *e,
                                              lambda_parser_t *p) {}

static void lambda_parser_parse_apply(lambda_ast_expression_t *e,
                                      lambda_parser_t *p) {}
