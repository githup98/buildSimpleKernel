#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>   // for size_t

#include <stdint.h>   // for uint16_t


#define HIGH 40
#define WIDTH 80

#define TDOS_MAX_PATH 108

void kernel_main();
void print(const char* str);
extern void problem();
#endif
