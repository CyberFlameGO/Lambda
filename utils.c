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

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void lambda_substring(char *src, char *dest, size_t start, size_t size) {
  memcpy(dest, &src[start], size);
  dest[size] = '\0';
}

extern char *lambda_read_file(const char *filename) {
  FILE *file = fopen(filename, "r");

  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);

  size_t buffer_len = ftell(file);
  char *buffer = calloc(buffer_len, buffer_len);

  fseek(file, 0, SEEK_SET);

  if (buffer)
    fread(buffer, 1, buffer_len, file);

  fclose(file);
  return buffer;
}

extern char *lambda_format(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *s = lambda_formatv(format, args);

  va_end(args);
  return s;
}

extern char *lambda_formatv(const char *format, va_list args) {
  va_list args0;
  va_copy(args0, args);

  size_t len = vsnprintf(NULL, 0, format, args) + 1;
  char *s = malloc(len);
  vsnprintf(s, len, format, args0);

  va_end(args0);
  return s;
}

// From
// https://stackoverflow.com/questions/779875/what-function-is-to-replace-a-substring-from-a-string-in-c
extern char *lambda_string_replace(char *original, char *pattern,
                                   char *replacement) {
  size_t replen = strlen(replacement);
  size_t patlen = strlen(pattern);
  size_t orilen = strlen(original);

  size_t patcnt = 0;
  char *oriptr;
  char *patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern);
       oriptr = patloc + patlen) {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t retlen = orilen + patcnt * (replen - patlen);
    char *returned = malloc(retlen + 1);
    returned[retlen] = '\0';

    if (returned != NULL) {
      // copy the original string,
      // replacing all the instances of the pattern
      char *retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern);
           oriptr = patloc + patlen) {
        size_t skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}