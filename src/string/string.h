#ifndef STRING_H
#define STRING_H
#include "stdbool.h"


int strlen(const char* str);
int strnlen(const char* str, int maxLen);
bool isDigit(char c);
int toNumericDigit(char c);

#endif
