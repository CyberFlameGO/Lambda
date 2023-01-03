#include "location.h"

extern void init_lambda_char_location(lambda_char_location_t *l, size_t index,
                                      size_t line, size_t column) {
  l->index = index;
  l->line = line;
  l->column = column;
}

extern void copy_lambda_char_location(lambda_char_location_t *src,
                                      lambda_char_location_t *dst) {
  dst->index = src->index;
  dst->line = src->line;
  dst->column = src->column;
}

extern void init_lambda_block_location(lambda_block_location_t *b,
                                       lambda_char_location_t *start,
                                       lambda_char_location_t *end) {
  b->start = start;
  b->end = end;
}

extern void
init_lambda_block_location_from_char_location(lambda_block_location_t *b,
                                              lambda_char_location_t *c) {
  b->start = c;
  init_lambda_char_location(b->end, c->index + 1, c->line, c->column + 1);
}

extern void init_lambda_two_chars_block_location(lambda_block_location_t *b,
                                                 lambda_char_location_t *c) {
  b->start = c;
  init_lambda_char_location(b->end, c->index + 2, c->line, c->column + 2);
}