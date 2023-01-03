#ifndef _LAMBDA_UTILS_H_
#define _LAMBDA_UTILS_H_

#include <stddef.h>

extern void lambda_substring(char *src, char *dest, size_t start, size_t size);

extern char *lambda_read_file(const char *filename);

#endif /* _LAMBDA_UTILS_H_ */