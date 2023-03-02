#include "memory.h"


void* memset(void* ptr, char c, size_t size)
{
	char* c_ptr = (char*) ptr;
	for(int i = 0; i < size ; i++)
	{
		c_ptr[i] = c;
	}
	return ptr;
}

int memcmp(void* s1, void* s2, int count) // compare 2 char only
{
	char* c1 = (char*) s1;
	char* c2 = (char*) s2;
	while(count-- > 0)
	{
		if(*c1++ != *c2++)
		{
			return c1[-1] < c2[-1] ? -1 : 1; //ternary case
		}
	}
	return 0;
}
