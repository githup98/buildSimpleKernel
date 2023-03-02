#include "string.h"

int strlen(const char* str)
{
	unsigned int i = 0;
	while(*str != 0)
	{
		i++;
		str ++; // create 1 byte
	}
	return i;
}

int strnlen(const char* str, int maxLen)
{
	int i = 0;
	for(i = 0; i < maxLen; i++)
	{
		if(str[i] == 0)
		{
			break;
		}
	}
	return i;
}

bool isDigit(char c)
{
	return (c >= 48) && (c <= 57); //return true or false
}

int toNumericDigit(char c)
{
	return c - 48 ; //convert char to num is digit format 0-9 in text

}


