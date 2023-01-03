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