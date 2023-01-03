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

#ifndef _LAMBDA_ERROR_HANDLER_H_
#define _LAMBDA_ERROR_HANDLER_H_

#include "error.h"
#include "line_offsets.h"
#include <stdbool.h>

typedef struct lambda_error_handler {
  const char *filename;
  char *source;

  lambda_line_offsets_t *line_offsets;

  lambda_error_t **errors;
  size_t errors_len;

  bool ok;
} lambda_error_handler_t;

extern void init_lambda_error_handler(lambda_error_handler_t *h,
                                      const char *filename, char *source);

extern void
lambda_error_handler_set_line_offsets_ptr(lambda_error_handler_t *h,
                                          lambda_line_offsets_t *line_offsets);

extern void lambda_error_handler_add_error(lambda_error_handler_t *h,
                                           char *message,
                                           lambda_block_location_t *location);

extern void lambda_error_handler_print_errors(lambda_error_handler_t *h);

extern void lambda_error_handler_clean_errors(lambda_error_handler_t *h);

#endif /* _LAMBDA_ERROR_HANDLER_H_ */