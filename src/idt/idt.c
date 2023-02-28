#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "io/io.h"

struct idt_desc idt_descriptors[TDOsTotalInterrupt] ; // declare array contain total number of interrupt
struct idtr_desc idtr_descriptor;

extern void idtLoad(struct idtr_desc *ptr);
extern void int21h();
extern void noInterrupt();


void idtZero()
{
	print("interrupt zero is involked!\n");
}

void int21hHandler()
{
	print("keyboard is pressed!\n");
	outb(0x20, 0x20); // this value notify interrupt is end,
					 // if not PIC will not gen interrupt signal a gain
}

void noInterruptHandler()
{
	outb(0x20, 0x20) ; //send 0x20 to port has address is 0x20
}


void idtSet(int interruptNo, void* address)
{
	struct idt_desc *desc = &idt_descriptors[interruptNo];  // pointe to element in idt_desc
	desc->offset1 = (uint32_t) address & 0x0000ffff;
	desc->selector = kernelCodeSelector; //point to 0x08
	desc->zero = 0x00;
	desc->type_attr = 0b11101110 ; //dpl = 3, p = 1, type gate: interrupt gate
	desc->offset2 = (uint32_t)address >> 16;
}

void idtInit()
{
	//remove area memory
	memset(idt_descriptors, 0, sizeof(idt_descriptors));

	idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
	//poin to idt_descriptor address
	idtr_descriptor.base  = (uint32_t)idt_descriptors;
	
	for(int j = 0; j < TDOsTotalInterrupt; j++)
	{
		idtSet(j, noInterrupt);  // noInterrupt is parse address of func
	}
	//assign function to interrupt routine
	////idtSet(0, idtZero);
	
	idtSet(0x20, int21h);
	//load idt

	idtLoad(&idtr_descriptor);

}
