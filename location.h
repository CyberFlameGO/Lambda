#ifndef _LAMBDA_LOCATION_H_
#define _LAMBDA_LOCATION_H_

#include <stddef.h>

typedef struct lambda_char_location {
  size_t index;
  size_t line, column;
} lambda_char_location_t;

extern void init_lambda_char_location(lambda_char_location_t *l, size_t index,
                                      size_t line, size_t column);

extern void copy_lambda_char_location(lambda_char_location_t *src,
                                      lambda_char_location_t *dst);

typedef struct lambda_block_location {
  lambda_char_location_t *start;
  lambda_char_location_t *end;
} lambda_block_location_t;

extern void init_lambda_block_location(lambda_block_location_t *b,
                                       lambda_char_location_t *start,
                                       lambda_char_location_t *end);

extern void
init_lambda_block_location_from_char_location(lambda_block_location_t *b,
                                              lambda_char_location_t *c);

extern void init_lambda_two_chars_block_location(lambda_block_location_t *b,
                                                 lambda_char_location_t *c);

#endif /* _LAMBDA_LOCATION_H_ */