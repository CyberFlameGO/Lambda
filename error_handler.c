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

#include "error_handler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

extern void init_lambda_error_handler(lambda_error_handler_t *h,
                                      const char *filename, char *source) {
  h->filename = filename;
  h->source = source;

  h->errors = malloc(0);
  h->errors_len = 0;

  h->ok = true;
}

extern void
lambda_error_handler_set_line_offsets_ptr(lambda_error_handler_t *h,
                                          lambda_line_offsets_t *line_offsets) {
  h->line_offsets = line_offsets;
}

extern void lambda_error_handler_add_error(lambda_error_handler_t *h,
                                           char *message,
                                           lambda_block_location_t *location) {
  h->ok = false;

  h->errors_len++;

  lambda_error_t *error = malloc(sizeof(lambda_error_t));

  init_lambda_error(error, message, location);

  h->errors = realloc(h->errors, h->errors_len * sizeof(lambda_error_t));
  h->errors[h->errors_len - 1] = error;
}

static void
lambda_error_handler_print_error_codeblock(lambda_error_handler_t *h,
                                           lambda_error_t *e) {
  char *line_number_str = lambda_format("%d", e->location->start->line);
  size_t line_number_str_size = strlen(line_number_str);

  char *spaces_before_bar = calloc(line_number_str_size + 3, 1);
  for (size_t i = 0; i < line_number_str_size + 2; i++) {
    spaces_before_bar[i] = ' ';
  }

  fprintf(stderr, "%s|\n %d | ", spaces_before_bar, e->location->start->line);

  size_t line_size =
      h->line_offsets->end_offsets[e->location->start->line - 1] -
      h->line_offsets->start_offsets[e->location->start->line - 1];

  char *line_buffer = malloc(line_size + 1);
  lambda_substring(h->source, line_buffer,
                   h->line_offsets->start_offsets[e->location->start->line - 1],
                   line_size);
  line_buffer[line_size] = '\0';

  char *new_buffer = lambda_string_replace(line_buffer, "\t", " ");
  fprintf(stderr, "%s\n", new_buffer);

  fprintf(stderr, "%s|", spaces_before_bar);

  size_t spaces_before_arrow = e->location->start->column + 1;
  for (size_t i = 0; i < spaces_before_arrow; i++) {
    fprintf(stderr, " ");
  }

  fprintf(stderr, "^");

  size_t tilda_symbols =
      e->location->end->column - e->location->start->column - 1;

  for (size_t i = 0; i < tilda_symbols; i++) {
    fprintf(stderr, "~");
  }

  fprintf(stderr, "\n\n");
}

static void lambda_error_handler_print_error(lambda_error_handler_t *h,
                                             lambda_error_t *e) {
  fprintf(stderr, "%s", h->filename);
  fprintf(stderr, ":%ld:%ld(-%ld:%ld): error: %s\n", e->location->start->line,
          e->location->start->column, e->location->end->line,
          e->location->end->column, e->message);

  lambda_error_handler_print_error_codeblock(h, e);
}

extern void lambda_error_handler_print_errors(lambda_error_handler_t *h) {
  for (size_t i = 0; i < h->errors_len; i++) {
    lambda_error_handler_print_error(h, h->errors[i]);
  }
}

extern void lambda_error_handler_clean_errors(lambda_error_handler_t *h) {
  for (size_t i = 0; i < h->errors_len; i++) {
    free(h->errors[i]->message);
    free(h->errors[i]->location->start);
    free(h->errors[i]->location->end);
    free(h->errors[i]->location);
    free(h->errors[i]);
  }
}