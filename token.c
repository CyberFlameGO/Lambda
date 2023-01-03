#include "token.h"

extern void init_lambda_token(lambda_token_t *t, lambda_token_kind_t kind,
                              lambda_block_location_t *location,
                              char *literal) {
  t->kind = kind;
  t->location = location;
  t->literal = literal;
}