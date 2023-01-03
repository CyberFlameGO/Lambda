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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: lambda_lex <filename>\n");
    return 1;
  }

  const char *filepath = argv[1];
  char *source = lambda_read_file(filepath);

  if (source == NULL) {
    fprintf(stderr, "unable to read the file\n");
    return 1;
  }

  lambda_lexer_t *l = malloc(sizeof(lambda_lexer_t));
  init_lambda_lexer(l, filepath, source);

  lambda_error_handler_t *h = malloc(sizeof(lambda_error_handler_t));
  init_lambda_error_handler(h, filepath, source);

  lambda_lexer_enable_error_handling(l, h);

  bool finished = false;

  for (;;) {
    lambda_token_t *t = malloc(sizeof(lambda_token_t));

    lambda_lexer_next_token(t, l);

    printf("tok(%d, %s, %ld:%ld(%ld)-%ld:%ld(%ld))\n", t->kind, t->literal,
           t->location->start->line, t->location->start->column,
           t->location->start->index, t->location->end->line,
           t->location->end->column, t->location->end->index);

    if (t->kind == LAMBDA_EOF_TOKEN_KIND) {
      finished = true;
    }

    free(t->location->start);
    free(t->location->end);
    free(t->location);
    free(t->literal);
    free(t);

    if (finished) {
      lambda_error_handler_print_errors(h);
      lambda_error_handler_clean_errors(h);
      free(h);
      lambda_line_offsets_free(l->line_offsets);

      free(l->location);
      free(l);
      free(source);
      return 0;
    }
  }
}