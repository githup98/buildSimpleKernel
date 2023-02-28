[BITS 32]
global _start
global problem
extern kernel_main  ; indecate that kernel_main will be define in c code

CODE_SEG equ 0x8
DATA_SEG equ 0x10

_start:
	mov ax, DATA_SEG
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax
	mov ebp, 0x00200000 ; why???
	mov esp, ebp
	
	;; Enable A20 line

	in al, 0x92
	or al, 2
	out 0x92, al	
	;;

	;; remap master PIC 
	;; tell to master base address 0x20 (for command, 0x21/(0xa1 for slave) for data)
	mov al, 00010001b
	out 0x20, al

	;; tell to master where IRQ should be start
	mov al, 0x20
	out 0x21, al   ; 0x21 for send data
	
	;; end of remap master PIC
	mov al, 0x01
	out 0x21, al
	;;;;;;;
	
	;; enalbe interrupt	 due to PIC will ignore signal when interrupt is gen first time
	sti
	
	
	call kernel_main ; call c code

	jmp $ ; do nothing (same as exit system call)

problem:
	;mov eax, 0x00
	;div eax
	int 0
times 512 - ($-$$) db 0   ; this line to ensure alignment due to, all data is put into
						  ; each sector
