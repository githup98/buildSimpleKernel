[BITS 32]
global _start

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

	call kernel_main ; call c code

	jmp $ ; do nothing (same as exit system call)

times 512 - ($-$$) db 0   ; this line to ensure alignment due to, all data is put into
						  ; each sector
