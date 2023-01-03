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

#ifndef _LAMBDA_AST_H_
#define _LAMBDA_AST_H_

#include "location.h"

typedef enum lambda_ast_expression_kind {
  LAMBDA_AST_VAR_EXPRESSION_KIND,
  LAMBDA_AST_APPLY_EXPRESSION_KIND,
  LAMBDA_AST_LAMBDA_EXPRESSION_KIND,
} lambda_ast_expression_kind_t;

typedef struct lambda_ast_var_expression {
  char *name;
} lambda_ast_var_expression_t;

typedef struct lambda_ast_apply_expression {
  struct lambda_ast_lambda_expression *function;
  struct lambda_ast_lambda_expression *arg;
} lambda_ast_apply_expression_t;

typedef struct lambda_ast_lambda_expression {
  char *arg;
  struct lambda_ast_lambda_expression *body;
} lambda_ast_lambda_expression_t;

typedef struct lambda_ast_expression {
  lambda_block_location_t *location;
  lambda_ast_expression_kind_t kind;
  union {
    lambda_ast_var_expression_t *var;
    lambda_ast_apply_expression_t *apply;
    lambda_ast_lambda_expression_t *lambda;
  } data;
} lambda_ast_expression_t;

extern void init_lambda_ast_var_expression(lambda_ast_expression_t *e,
                                           lambda_block_location_t *location,
                                           char *name);

extern void init_lambda_ast_apply_expression(lambda_ast_expression_t *e,

                                             lambda_block_location_t *location,
                                             lambda_ast_expression_t *function,
                                             lambda_ast_expression_t *arg);

extern void init_lambda_ast_lambda_expression(lambda_ast_expression_t *e,
                                              lambda_block_location_t *location,
                                              char *arg,
                                              lambda_ast_expression_t *body);

typedef struct lambda_ast_let_statement {
  char *name;
  lambda_ast_expression_t *value;
} lambda_ast_let_statement_t;

typedef enum lambda_ast_statement_kind {
  LAMBDA_AST_LET_STATEMENT_KIND,
  LAMBDA_AST_EXPRESSION_STATEMENT_KIND,
} lambda_ast_statement_kind_t;

typedef struct lambda_ast_statement {
  lambda_block_location_t *location;
  lambda_ast_statement_kind_t kind;
  union {
    lambda_ast_expression_t *expression;
    lambda_ast_let_statement_t *let;
  } data;
} lambda_ast_statement_t;

void init_lambda_ast_expression_statement(lambda_ast_statement_t *stmt,
                                          lambda_ast_expression_t *expression);

void init_lambda_ast_let_statement(lambda_ast_statement_t *stmt,
                                   lambda_block_location_t *location,
                                   char *name, lambda_ast_expression_t *value);

typedef struct lambda_ast {
  lambda_ast_statement_t **statements;
  size_t statements_len;
} lambda_ast_t;

void init_lambda_ast(lambda_ast_t *ast);

void lambda_ast_add_statement(lambda_ast_t *ast,
                              lambda_ast_statement_t *statement);

void lambda_ast_clean_statements(lambda_ast_t *ast);

#endif /* _LAMBDA_AST_H_ */