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

  bool finished = false;

  for (;;) {
    lambda_token_t *t = malloc(sizeof(lambda_token_t));

    lambda_lexer_next_token(t, l);

    printf("tok(%d, %s, %ld:%ld-%ld:%ld)\n", t->kind, t->literal,
           t->location->start->line, t->location->start->column,
           t->location->end->line, t->location->end->column);

    if (t->kind == LAMBDA_EOF_TOKEN_KIND) {
      finished = true;
    }

    free(t->location->start);
    free(t->location->end);
    free(t->location);
    free(t->literal);
    free(t);

    if (finished) {
      free(l->location);
      free(l);
      free(source);
      return 0;
    }
  }
}