#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "string/string.h"

#include "fs/pParser.h"

uint16_t* videoMem = 0;
uint16_t row = 0;
uint8_t col = 0;

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

static struct paging4gbChunk *kernelChunk = 0;
void kernel_main()
{
	terminalInit();
	printChar ('A',15);
	print("hello\n world");

	// init kernel heap
	kheapInit();
	
	//search and init primary disk (0)
	diskSearchAndInit();

	//init the interrupt descriptors tabel
	idtInit();
	//problem();
	////outb(0x60, 0xff);  // out 1 byte to 0x60 port
	
	//set up page
	kernelChunk = pagingNew4gb(PAGING_IS_PRESENT | PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL); 
	
	//load page
	pagingSwitch(paging4gbChunkGetDirectory(kernelChunk));

	// map kernelChunk to kzalloc (which func return physical address)
	char* ptr = kzalloc(4096); //ptr will point to 0x1000

	pageSet(paging4gbChunkGetDirectory(kernelChunk),(void*)0x1000, (uint32_t)ptr | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE);

	// enable page
	enablePage();

	//read secttor
	char buf[512];
	diskReadBlock(diskGet(0), 0, 1, buf);

	// enable interrupt
	enableInt();

	struct pathRoot* root;
	const char* filename = "0:/test/insert/shell.exe";
	root = pathParserParser(filename, NULL);
	pathParserFree(root);
}
