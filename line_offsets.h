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

#ifndef _LAMBDA_LINE_OFFSETS_H_
#define _LAMBDA_LINE_OFFSETS_H_

#include <stddef.h>

typedef struct lambda_line_offsets {
  size_t *start_offsets;
  size_t start_offsets_len;

  size_t *end_offsets;
  size_t end_offsets_len;
} lambda_line_offsets_t;

extern void init_lambda_line_offsets(lambda_line_offsets_t *o);

extern void lambda_line_offsets_add_start_offset(lambda_line_offsets_t *os,
                                                 size_t start_offset);

extern void lambda_line_offsets_add_end_offset(lambda_line_offsets_t *os,
                                               size_t end_offset);

extern void lambda_line_offsets_free(lambda_line_offsets_t *o);

#endif /* _LAMBDA_LINE_OFFSETS_H_ */