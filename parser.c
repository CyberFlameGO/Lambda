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
#include "utils.h"

static void lambda_parser_consume_token(lambda_parser_t *p);

static void lambda_parser_consume_remaining_tokens(lambda_parser_t *p);

static void lambda_parser_parse_statement(lambda_ast_statement_t *s,
                                          lambda_parser_t *p);

static void lambda_parser_parse_let_statement(lambda_ast_statement_t *s,
                                              lambda_parser_t *p);

static void lambda_parser_parse_expression(lambda_ast_expression_t *e,
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

static bool lambda_parser_check_current_token(lambda_parser_t *p,
                                              lambda_token_kind_t expected,
                                              char *message);

static bool lambda_parser_check_expect_token(lambda_parser_t *p,
                                             lambda_token_kind_t expected,
                                             char *message);

void init_lambda_parser(lambda_parser_t *p, lambda_lexer_t *l) {
  p->lexer = l;
  p->error_handler = l->error_handler;
  p->current_token = malloc(sizeof(lambda_token_t));
  p->peek_token = malloc(sizeof(lambda_token_t));
  lambda_parser_consume_token(p);
  lambda_parser_consume_token(p);
}

void lambda_parser_parse(lambda_ast_t *ast, lambda_parser_t *p);

static void lambda_parser_consume_token(lambda_parser_t *p) {
  p->current_token = p->peek_token;
  lambda_lexer_next_token(p->peek_token, p->lexer);
}

static void lambda_parser_consume_remaining_tokens(lambda_parser_t *p) {
  while (p->current_token->kind != LAMBDA_EOF_TOKEN_KIND) {
    lambda_parser_consume_token(p);
  }
}

static void lambda_parser_parse_statement(lambda_ast_statement_t *s,
                                          lambda_parser_t *p) {
  if (p->current_token->kind == LAMBDA_LET_KEYWORD_TOKEN_KIND) {
    lambda_parser_parse_let_statement(s, p);
  } else {
    lambda_parser_parse_expression_statement(s, p);
  }
}

static void lambda_parser_parse_let_statement(lambda_ast_statement_t *s,
                                              lambda_parser_t *p) {
  lambda_char_location_t *start_location =
      malloc(sizeof(lambda_char_location_t));
  copy_lambda_char_location(p->current_token->location->start, start_location);
  lambda_parser_consume_token(p); // LET

  char *name = p->current_token->literal;
  if (!lambda_parser_check_current_token(p, LAMBDA_IDENTIFIER_TOKEN_KIND,
                                         "expected name for let statement")) {
    s = NULL;
  }

  lambda_ast_expression_t *expression = malloc(sizeof(lambda_ast_expression_t));

  lambda_char_location_t *end_location = malloc(sizeof(lambda_char_location_t));
  copy_lambda_char_location(p->current_token->location->end, end_location);

  lambda_parser_parse_expression(expression, p);

  s->kind = LAMBDA_AST_LET_STATEMENT_KIND;

  s->location->start = start_location;
  s->location->end = end_location;

  s->data.let->name = name;
  s->data.let->value = expression;
}

static void lambda_parser_parse_expression(lambda_ast_expression_t *e,
                                           lambda_parser_t *p) {}

static void lambda_parser_parse_expression_statement(lambda_ast_statement_t *s,
                                                     lambda_parser_t *p) {
  lambda_ast_expression_t *expression = malloc(sizeof(lambda_ast_expression_t));
  lambda_parser_parse_expression(expression, p);

  lambda_ast_statement_t *s = malloc(sizeof(lambda_ast_statement_t));
  s->kind = LAMBDA_AST_EXPRESSION_STATEMENT_KIND;
  s->data.expression = expression;
}

static void lambda_parser_parse_unary(lambda_ast_expression_t *e,
                                      lambda_parser_t *p) {
  if (p->current_token->kind == LAMBDA_LPAREN_TOKEN_KIND) {
    lambda_parser_parse_parenthesised(e, p);
  } else if (p->current_token->kind == LAMBDA_IDENTIFIER_TOKEN_KIND) {
    char *name = lambda_format(p->current_token->literal);
    e->kind = LAMBDA_AST_VAR_EXPRESSION_KIND;
    e->data.var->name = name;
  } else if (p->current_token->kind == LAMBDA_LAMBDA_TOKEN_KIND) {
    lambda_parser_parse_lambda(e, p);
  }

  lambda_block_location_t *bc = malloc(sizeof(lambda_block_location_t));
  bc->start = malloc(sizeof(lambda_char_location_t));
  bc->end = malloc(sizeof(lambda_char_location_t));
  copy_lambda_block_location(p->peek_token->location, bc);
  lambda_error_handler_add_error(p->error_handler,
                                 lambda_format("unexpected token"), bc);
}

static void lambda_parser_parse_lambda(lambda_ast_expression_t *e,
                                       lambda_parser_t *p) {
  lambda_parser_consume_token(p); // 'λ' / '/' / 'lambda'

  if (!lambda_parser_check_current_token(
          p, LAMBDA_IDENTIFIER_TOKEN_KIND,
          lambda_format(
              "expected identifier (arg) in abstraction expression"))) {
    return NULL;
  }

  char *arg = lambda_format(p->current_token->literal);

  lambda_parser_consume_token(p); // arg

  if (!lambda_parser_check_current_token(
          p, LAMBDA_PERIOD_OR_RIGHTARROW_TOKEN_KIND,
          lambda_format("expected '->' or '.' in abstraction expression"))) {
    return NULL;
  }

  e->kind = LAMBDA_AST_LAMBDA_EXPRESSION_KIND;

  if (p->current_token->kind == LAMBDA_PERIOD_OR_RIGHTARROW_TOKEN_KIND) {
    lambda_parser_parse_expression(e->data.lambda->body, p);

    e->data.lambda->arg = arg;
  } else if (p->current_token->kind == LAMBDA_IDENTIFIER_TOKEN_KIND) {
    lambda_parser_parse_lambda(e->data.lambda->body, p);

    e->data.lambda->arg = arg;
  } else {
    lambda_block_location_t *bc = malloc(sizeof(lambda_block_location_t));
    bc->start = malloc(sizeof(lambda_char_location_t));
    bc->end = malloc(sizeof(lambda_char_location_t));
    copy_lambda_block_location(p->peek_token->location, bc);
    lambda_error_handler_add_error(
        p->error_handler,
        lambda_format(
            "expected '->', '.' or identifier in abstraction expression"),
        bc);
  }
}

static void lambda_parser_parse_parenthesised(lambda_ast_expression_t *e,
                                              lambda_parser_t *p) {
  lambda_parser_consume_token(p); // '('

  lambda_parser_parse_expression(e, p);

  if (!lambda_parser_check_current_token(
          p, LAMBDA_RPAREN_TOKEN_KIND,
          lambda_format("expected ')' in parenthesised expression"))) {
    return NULL;
  }

  lambda_parser_consume_token(p); // ')'
}

static void lambda_parser_parse_apply(lambda_ast_expression_t *e,
                                      lambda_parser_t *p) {}

static bool lambda_parser_check_current_token(lambda_parser_t *p,
                                              lambda_token_kind_t expected,
                                              char *message) {
  if (p->current_token->kind != expected) {
    lambda_block_location_t *bc = malloc(sizeof(lambda_block_location_t));
    bc->start = malloc(sizeof(lambda_char_location_t));
    bc->end = malloc(sizeof(lambda_char_location_t));
    copy_lambda_block_location(p->current_token->location, bc);
    lambda_error_handler_add_error(p->error_handler, message, bc);
    return false;
  }

  return true;
}

static bool lambda_parser_check_expect_token(lambda_parser_t *p,
                                             lambda_token_kind_t expected,
                                             char *message) {
  if (p->peek_token->kind != expected) {
    lambda_block_location_t *bc = malloc(sizeof(lambda_block_location_t));
    bc->start = malloc(sizeof(lambda_char_location_t));
    bc->end = malloc(sizeof(lambda_char_location_t));
    copy_lambda_block_location(p->peek_token->location, bc);
    lambda_error_handler_add_error(p->error_handler, message, bc);
    return false;
  }

  return true;
}