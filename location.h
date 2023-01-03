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