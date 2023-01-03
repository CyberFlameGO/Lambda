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

#include "ast.h"
#include <stdlib.h>

extern void init_lambda_ast_var_expression(lambda_ast_expression_t *e,
                                           lambda_block_location_t *location,
                                           char *name) {
  e->location = location;
  e->kind = LAMBDA_AST_VAR_EXPRESSION_KIND;
  e->data.var->name = name;
}

extern void init_lambda_ast_apply_expression(lambda_ast_expression_t *e,
                                             lambda_block_location_t *location,
                                             lambda_ast_expression_t *function,
                                             lambda_ast_expression_t *arg) {
  e->location = location;
  e->kind = LAMBDA_AST_APPLY_EXPRESSION_KIND;
  e->data.apply->function = function;
  e->data.apply->arg = arg;
}

extern void init_lambda_ast_lambda_expression(lambda_ast_expression_t *e,
                                              lambda_block_location_t *location,
                                              char *arg,
                                              lambda_ast_expression_t *body) {
  e->location = location;
  e->kind = LAMBDA_AST_LAMBDA_EXPRESSION_KIND;
  e->data.lambda->arg = arg;
  e->data.lambda->body = body;
}

void init_lambda_ast_expression_statement(lambda_ast_statement_t *stmt,
                                          lambda_ast_expression_t *expression) {
  stmt->kind = LAMBDA_AST_EXPRESSION_STATEMENT_KIND;
  stmt->location = expression->location;
  stmt->data.expression = expression;
}

void init_lambda_ast_let_statement(lambda_ast_statement_t *stmt,
                                   lambda_block_location_t *location,
                                   char *name, lambda_ast_expression_t *value) {
  stmt->kind = LAMBDA_AST_LET_STATEMENT_KIND;
  stmt->location = location;
  stmt->data.let->name = name;
  stmt->data.let->value = value;
}

void init_lambda_ast(lambda_ast_t *ast) {
  ast->statements_len = 0;
  ast->statements = malloc(0);
}

void lambda_ast_add_statement(lambda_ast_t *ast,
                              lambda_ast_statement_t *statement) {
  ast->statements_len++;
  ast->statements = (lambda_ast_statement_t **)realloc(
      ast->statements, ast->statements_len * sizeof(lambda_ast_statement_t *));
  ast->statements[ast->statements_len - 1] = statement;
}

void lambda_ast_clean_statements(lambda_ast_t *ast) {}