ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 1M;
	.text :	ALIGN(4096) 
	{
		*(.text)
	}

	.rodata : ALIGN(4096)
	{
		*(.rodata)
	}
	
	.data : ALIGN(4096)
	{
		*(.data)
	}

	.bss : ALIGN(4096)
	{
		*(COMMON)
		*(.bss)
	}
	
	/*this segment is put here due to it used to alignment for lastt file 
	(see "all" lable in Makefile),
	 if put it at then first, it can cause alignment for above .text ( c code) 
	 segment
	*/
	.asm : ALIGN(4096)
	{
		*(.asm)
	}
}
