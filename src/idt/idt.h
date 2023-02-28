#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_desc // create interrupt descriptors table
{
	uint16_t offset1; // 0-15 lower bits (entry point of interrupt services routine)
	uint16_t selector; //must point to a valid code segment
	uint8_t zero;      //reserve
	uint8_t type_attr; //type gate, dpl (user space), p- preesent bits
	uint16_t offset2; //16-31 higher bits address

}__attribute__((packed));


struct idtr_desc   // create idt register desc
{
	uint16_t limit ; //size of idt - 1
	uint32_t base; // the linear address of idt

}__attribute__((packed));
extern void idtInit();
#endif
