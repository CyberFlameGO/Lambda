#ifndef _LAMBDA_LEXER_H_
#define _LAMBDA_LEXER_H_

#include "location.h"
#include "token.h"

typedef struct lambda_lexer {
  const char *filename;
  char *source;
  size_t source_len;

  lambda_char_location_t *location;
} lambda_lexer_t;

extern void init_lambda_lexer(lambda_lexer_t *l, const char *filename,
                              char *source);

extern void lambda_lexer_next_token(lambda_token_t *t, lambda_lexer_t *l);

#endif /* _LAMBDA_LEXER_H_ */