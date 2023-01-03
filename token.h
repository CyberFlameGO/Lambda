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