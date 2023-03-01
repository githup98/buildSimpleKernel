#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

uint16_t* videoMem = 0;
uint16_t row = 0;
uint8_t col = 0;
size_t strlen(const char* str)
{
	size_t len = 0;
	while(str[len])
	{
		len ++;
	}
	return len;
}


void terminalInit()
{
	//width's terminal of qemu is 80, 
	uint8_t x, y;
	videoMem = (uint16_t *)0xB8000;
	for(y = 0; y < HIGH; y ++)
	{
		for(x = 0; x < WIDTH; x ++)
		{
			videoMem[y * HIGH + x] = (3 << 8) | ' ';
		}
	}
}

void printChar(char c, uint8_t color)
{
	videoMem[0] = (color << 8) | c ;
}

void print(const char* str)
{
	uint8_t color = 15;
	uint16_t len = strlen(str);
	uint8_t i;
	

	for(i = 0; i < len; i ++)
	{
		col++;
		if(col >= 80)
		{
			col = 0;
			row ++;
		}
		if (str[i] == '\n')
		{
			row ++;
			if(row >= 80)
			{
				row = 0;
			}
			col = 0;
			continue;
		}
		videoMem[row* WIDTH + col] = (color << 8) | str[i];
	}
}


void kernel_main()
{
	terminalInit();
	printChar ('A',15);
	print("hello\n world");

	// init kernel heap
	kheapInit();
	
	//init the interrupt descriptors tabel
	idtInit();
	//problem();
	////outb(0x60, 0xff);  // out 1 byte to 0x60 port

	void* ptr = kmalloc(50);
	void* ptr2 = kmalloc(5000);
	void* ptr3 = kmalloc(5600);
	kfree(ptr);
	void* ptr4 = kmalloc(50);
	if(ptr || ptr2 || ptr3 || ptr4)
	{
		
	}
}
