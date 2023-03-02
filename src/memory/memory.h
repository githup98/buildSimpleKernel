#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>

void* memset(void* ptr, char c, size_t size);
int memcmp(void* s1, void* s2, int count);
#endif
